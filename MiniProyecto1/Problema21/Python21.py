import sys
import matplotlib
matplotlib.use('QtAgg')
from PyQt6.QtCore import QTimer, Qt
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from scipy.signal import convolve2d

import numpy as np
import serial
import time

class GolGame:
    def __init__(self, size):
        self.size = size
        self.grid = np.random.choice([0, 1], size*size, p=[0.8, 0.2]).reshape(size, size)

    def update(self):
        kernel = np.array([[1, 1, 1],
                           [1, 0, 1],
                           [1, 1, 1]])
        convolved = convolve2d(self.grid, kernel, mode='same', boundary='fill')
        birth = (convolved == 3) & (self.grid == 0)
        survive = ((convolved == 2) | (convolved == 3)) & (self.grid == 1)
        self.grid[:, :] = 0
        self.grid[birth | survive] = 1

class GolWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.initUI()

    def initUI(self):
        self.figure = Figure()
        self.ax = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self.figure)

        layout = QVBoxLayout()
        layout.addWidget(self.canvas)
        self.setLayout(layout)

        self.size = 100
        self.game = GolGame(self.size)
        self.im = self.ax.imshow(self.game.grid, cmap='gray')
        self.ax.axis('on')

    def update_game(self):
        self.game.update()
        self.im.set_array(self.game.grid)
        self.canvas.draw()


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()
        self.setup_serial()

    def initUI(self):
        self.setWindowTitle("Game of Life")
        self.setGeometry(100, 100, 800, 800)

        self.gol_widget = GolWidget(self)
        self.setCentralWidget(self.gol_widget)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_game)
        self.timer.start(100) # 1 segundo

        self.timerArdu = QTimer(self)
        self.timerArdu.timeout.connect(self.LecturaArduino)
        self.timerArdu.start(1000) # 10 segundo


    def LecturaArduino(self):
        try:
            self.alive_cells =  '{:04d}'.format(np.sum(self.gol_widget.game.grid))
            self.arduino.write(str(self.alive_cells).encode()) # Le manda la información de las celulas vivas a arduino.
            if self.arduino.in_waiting > 0:
                self.mensaje = self.arduino.readline().decode().strip()  #Recibe, lee y almacena un mensaje mandado desde arduino
                print(self.mensaje,type(self.mensaje))
                if self.mensaje == "r":
                    self.reset_game()
                    self.mensaje = " "    
            self.last_update_time = time.time()  # Actualizar el tiempo de la última actualización
        except serial.SerialException as e:
            print(f"Error al enviar datos a Arduino: {e}")


    def setup_serial(self):
        try:
            self.arduino = serial.Serial('COM6', 9600)  
            self.last_update_time = time.time()
        except serial.SerialException as e:
            print(f"Error al abrir el puerto serial: {e}")
            sys.exit(1)


    def update_game(self):
        self.gol_widget.update_game()
        
        # Enviar la cantidad de células vivas a Arduino cada vez que se actualice el juego
        self.alive_cells = '{:04d}'.format(np.sum(self.gol_widget.game.grid))

        print(self.alive_cells)


    def reset_game(self):
        self.gol_widget.game.grid = np.random.choice([0, 1], self.gol_widget.size*self.gol_widget.size, p=[0.8, 0.2]).reshape(self.gol_widget.size,self.gol_widget.size)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key.Key_R:  # Si se presiona la tecla 'R'
            self.reset_game()

def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())

if __name__ == '__main__':
    main()
