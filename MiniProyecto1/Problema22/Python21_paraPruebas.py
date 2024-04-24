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

def NumerosABooleano(arr):
    # Nos permite tomar algun vector y transformar sus elementos en 1 y en 0 dependiendo de si es mayor a 0 o no los transforma en 1 o een 0.
    binary_arr = (arr != 0).astype(int)
    return binary_arr

def FuncionBomba(matriz, bomba, x, y):
    """
    Nos permite crear una matriz que se aplica y reemplaza los elementos de otra matriz(bomba) en cierta posición (x,y) , donde los indices dan la vuelta.

    """
    # dimensiones 
    bomba_rows, bomba_cols = bomba.shape

    rows, cols = matriz.shape
    for i in range(bomba_rows):
        for j in range(bomba_cols):
            idx_i = (i + x) % bomba_rows  # Estos signos % son los que permiten aplicar 
            idx_j = (j + y) % bomba_cols
            matriz[(i + x) % rows, (j + y) % cols] = bomba[idx_i, idx_j]

    return matriz

def FuncionCura(matriz, bomba, x, y):
    """
    Nos permite crear una matriz que se aplica y reemplaza los elementos de otra matriz(bomba) en cierta posición (x,y) , donde los indices dan la vuelta.

    """
    # dimensiones 
    bomba_rows, bomba_cols = bomba.shape

    rows, cols = matriz.shape
    for i in range(bomba_rows):
        for j in range(bomba_cols):
            idx_i = (i + x) % bomba_rows  # Estos signos % son los que permiten aplicar 
            idx_j = (j + y) % bomba_cols
            if( matriz[(i + x) % rows, (j + y) % cols] == 0):
                matriz[(i + x) % rows, (j + y) % cols] = 70
            elif( matriz[(i + x) % rows, (j + y) % cols] > 0):
                matriz[(i + x) % rows, (j + y) % cols] = matriz[(i + x) % rows, (j + y) % cols] +50
                if (matriz[(i + x) % rows, (j + y) % cols] > 100): 
                    matriz[(i + x) % rows, (j + y) % cols] = 100

    return matriz

class GolGame:
    
    def __init__(self, size):
        self.size = size
        self.grid = np.random.choice([0, 100], size*size, p=[0.8, 0.2]).reshape(size, size)
        self.temp = 0

    def update(self):
        kernel = np.array([[1, 1, 1],
                           [1, 0, 1],
                           [1, 1, 1]])
        convolved = convolve2d(NumerosABooleano(self.grid), kernel, mode='same', boundary='fill')
        birth = (convolved == 3) & (self.grid == 0)
        survive = ((convolved == 2) | (convolved == 3)) & (NumerosABooleano(self.grid) == 1)
        stress = ((convolved < 2) | (convolved > 3)) & (NumerosABooleano(self.grid) == 1)
        self.grid[stress] = self.grid[stress] - 30
        self.grid[birth] = 100
        self.grid[survive] =  self.grid[survive] 
        self.grid[self.grid < 0] = 0
        self.grid[self.grid > 100] = 100
        """if (self.temp  > 10):
            self.grid[survive] =  self.grid[survive] + 10
        elif (self.temp  < 10):
            self.grid[survive] = self.grid[survive] - 10
        self.temp += 1"""

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
        self.im = self.ax.imshow(self.game.grid, cmap='viridis', vmin=0, vmax=100)
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
        self.alive_cells = '{:04d}'.format(np.sum(NumerosABooleano(self.gol_widget.game.grid)))

        print(self.alive_cells)


    def reset_game(self):
        self.gol_widget.game.grid = np.random.choice([0, 1], self.gol_widget.size*self.gol_widget.size, p=[0.8, 0.2]).reshape(self.gol_widget.size,self.gol_widget.size)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key.Key_R:  # Si se presiona la tecla 'R'
            self.reset_game()
        elif event.key() == Qt.Key.Key_B:
            self.bomb_game()
        elif event.key() == Qt.Key.Key_H:
            self.heal_game()
        event.accept()


    def bomb_game(self):
        xrand =  np.random.randint(1, 101)
        yrand =  np.random.randint(1, 101)
        bomba = np.zeros((21, 21))
        self.gol_widget.game.grid = FuncionBomba(self.gol_widget.game.grid, bomba , xrand , yrand)


    def heal_game(self):
        xrand =  np.random.randint(1, 101)
        yrand =  np.random.randint(1, 101)
        bomba = 100*np.ones((21, 21))
        self.gol_widget.game.grid = FuncionCura(self.gol_widget.game.grid, bomba , xrand , yrand)



def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())

if __name__ == '__main__':
    main()
