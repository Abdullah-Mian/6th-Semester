# -*- coding: utf-8 -*-
"""simple_linear_regression.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/drive/1GrAAGr4ORy4WZf6B9dpXHX23xE3qOGE7

# Simple Linear Regression

## Importing the libraries
"""

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

"""## Importing the dataset"""

dataset = pd.read_excel("/content/Salary_Data.xlsx")
X = dataset.iloc[:, :-1].values
y = dataset.iloc[:, -1].values

print("valeus of X")
print(X)
print("valeus of y")
print(y)

"""## Splitting the dataset into the Training set and Test set"""

from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 1/3, random_state = 0)
# import sklearn.model_selection
# X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(X, y, test_size = 1/3, random_state =0)

print((X_train))

""" Training the Simple Linear Regression model on the Training set"""

from sklearn.linear_model import LinearRegression
regressor = LinearRegression()
regressor.fit(X_train, y_train)

"""## Predicting the Test set results"""

y_pred = regressor.predict(X_test)

"""## Visualising the Training set results"""

plt.scatter(X_train, y_train, color = 'red')
plt.plot(X_train, regressor.predict(X_train), color = 'green')
plt.title('Salary vs Experience (Training set)')
plt.xlabel('Years of Experience')
plt.ylabel('Salary')
plt.show()

"""## Visualising the Test set results"""

plt.scatter(X_test, y_test, color = 'red')
plt.plot(X_train, regressor.predict(X_train), color = 'blue')

plt.title('Salary vs Experience (Test set)')
plt.xlabel('Years of Experience')
plt.ylabel('Salary')
plt.grid()
plt.show()

regressor.predict([[6]])



print(regressor.coef_)

print(regressor.intercept_)