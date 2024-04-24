import numpy as np

def apply_mask(matrix, mask, x, y):
    """
    Apply a mask over a matrix starting from position (x, y).
    """
    # Get the dimensions of the mask
    mask_rows, mask_cols = mask.shape

    # Apply the mask over the matrix
    matrix[x:x+mask_rows, y:y+mask_cols] = mask
# Example usage
# Create a matrix
matrix = np.zeros((22, 22))

# Create a mask
mask = 100*np.ones((21, 21))
mask[10, :] = 1  # Set a row to 1 as an example, you can customize the mask as needed
np.set_printoptions(threshold=np.inf)
# Apply the mask over the matrix starting from position (x=20, y=30)
apply_mask(matrix, mask, 1, 1)

print(matrix)