import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.metrics import r2_score, mean_squared_error, mean_absolute_error
from sklearn.model_selection import train_test_split
import pandas as pd
import matplotlib.pyplot as plt

# Load the dataset
data = pd.read_csv('SalaryData.csv')
X = data['YearsExperience'].values.reshape(-1, 1)  # Independent variable
y = data['Salary'].values                         # Dependent variable

# Split the dataset into training (70%) and test (30%) sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Manual implementation of simple linear regression
def manual_linear_regression(X_train, y_train):
    n = len(X_train)
    X_flat = X_train.flatten()
    
    # Calculate necessary sums
    sum_x = np.sum(X_flat)
    sum_y = np.sum(y_train)
    sum_xy = np.sum(X_flat * y_train)
    sum_x2 = np.sum(X_flat ** 2)
    
    # Calculate slope (b1) and intercept (b0)
    b1 = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x ** 2)
    b0 = (sum_y - b1 * sum_x) / n
    
    return b0, b1

# Prediction function for manual implementation
def predict(X, b0, b1):
    return b0 + b1 * X.flatten()

# Train the manual model
manual_b0, manual_b1 = manual_linear_regression(X_train, y_train)

# Train the sklearn LinearRegression model
sklearn_model = LinearRegression()
sklearn_model.fit(X_train, y_train)

# Make predictions on the test set
manual_pred = predict(X_test, manual_b0, manual_b1)
sklearn_pred = sklearn_model.predict(X_test)

# Function to calculate performance metrics
def calculate_metrics(y_true, y_pred):
    rmse = np.sqrt(mean_squared_error(y_true, y_pred))
    mae = mean_absolute_error(y_true, y_pred)
    r2 = r2_score(y_true, y_pred)
    return rmse, mae, r2

# Calculate metrics for both models
manual_rmse, manual_mae, manual_r2 = calculate_metrics(y_test, manual_pred)
sklearn_rmse, sklearn_mae, sklearn_r2 = calculate_metrics(y_test, sklearn_pred)

# Print performance metrics
print("=== Model Performance on Test Set ===")
print("\nManual Implementation:")
print(f"RMSE: {manual_rmse:.2f}")
print(f"MAE: {manual_mae:.2f}")
print(f"R²: {manual_r2:.4f}")
print("\nsklearn Implementation:")
print(f"RMSE: {sklearn_rmse:.2f}")
print(f"MAE: {sklearn_mae:.2f}")
print(f"R²: {sklearn_r2:.4f}")

# Print coefficients
print("\n=== Coefficients ===")
print("Manual Linear Regression:")
print(f"Intercept (b0): {manual_b0:.2f}")
print(f"Slope (b1): {manual_b1:.2f}")
print("\nsklearn Linear Regression:")
print(f"Intercept: {sklearn_model.intercept_:.2f}")
print(f"Slope: {sklearn_model.coef_[0]:.2f}")

# Plot 1: Manual Linear Regression
plt.figure(1)
plt.scatter(X_train, y_train, color='blue', label='Training data')
plt.scatter(X_test, y_test, color='orange', label='Test data')
X_range = np.linspace(X.min(), X.max(), 100).reshape(-1, 1)
plt.plot(X_range, manual_b0 + manual_b1 * X_range.flatten(), color='red', label='Manual regression')
plt.xlabel('Years of Experience')
plt.ylabel('Salary')
plt.title('Manual Linear Regression')
plt.legend()
plt.show()
# plt.savefig('manual_regression.png')
# plt.close()

# Plot 2: sklearn Linear Regression
plt.figure(2)
plt.scatter(X_train, y_train, color='blue', label='Training data')
plt.scatter(X_test, y_test, color='orange', label='Test data')
plt.plot(X_range, sklearn_model.predict(X_range), color='green', label='sklearn regression')
plt.xlabel('Years of Experience')
plt.ylabel('Salary')
plt.title('sklearn Linear Regression')
plt.legend()
plt.show()
# plt.savefig('sklearn_regression.png')
# plt.close()