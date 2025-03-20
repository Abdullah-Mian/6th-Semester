import numpy as np
import matplotlib.pyplot as plt

# Sample data
X = np.array([2,9,5,5,3,7,1,8,6,2])
Y = np.array([69,98,82,77,71,84,55,94,84,64])

# Calculate coefficients
n = len(X)
sum_x = np.sum(X)
sum_y = np.sum(Y)
sum_xy = np.sum(X * Y)
sum_x2 = np.sum(X ** 2)

b1 = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x ** 2)
b0 = (sum_y - b1 * sum_x) / n

# Predictions and residuals
Y_pred = b0 + b1 * X
residuals = Y - Y_pred

# Correct metrics calculation
SSE = np.sum(residuals**2)
SST = np.sum((Y - np.mean(Y))**2)
SSR = SST - SSE
R_squared = SSR / SST

print(f"Regression Equation: y = {b0:.2f} + {b1:.2f}x")
print(f"SSE: {SSE:.2f}")
print(f"SSR: {SSR:.2f}")
print(f"SST: {SST:.2f}")
print(f"R²: {R_squared:.4f}")

# Visualization
plt.figure(figsize=(12, 5))

# Regression plot
plt.subplot(1, 2, 1)
plt.scatter(X, Y, color='blue', label='Original Data')
X_extended = np.linspace(0, 15, 100)
plt.plot(X_extended, b0 + b1 * X_extended, color='red', 
         label=f'Regression Line\ny = {b0:.2f} + {b1:.2f}x\nR² = {R_squared:.3f}')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Regression Line')
plt.legend()

# Residual plot
plt.subplot(1, 2, 2)
plt.scatter(Y_pred, residuals, color='green')
plt.axhline(y=0, color='red', linestyle='--')
plt.xlabel('Predicted Values')
plt.ylabel('Residuals')
plt.title('Residual Analysis')
plt.tight_layout()
plt.show()