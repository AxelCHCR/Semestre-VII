import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression as LR
from sklearn.metrics import accuracy_score as acc
from sklearn.neural_network import MLPClassifier as NN
from sklearn.neighbors import KNeighborsClassifier as KNN   
# Cargar el dataset
dataset = pd.read_csv('diabetes.csv')
#print(dataset.head())

x = dataset.drop('Outcome', axis=1) # Features
y = dataset['Outcome'] # Target
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=42)


neigh = KNN(n_neighbors=2)
neigh.fit(x_train, y_train)
print(f" features: {x_test[0:1]}")
print(f"a: {[x_test[0:1].values[0]]}")
print(neigh.predict([x_test[0:1].values[0]]))

log_reg = LR(max_iter=700)
log_reg.fit(x_train, y_train)
log_reg_pred = log_reg.predict(x_test)
log_reg_accuracy = acc(y_test, log_reg_pred)
print("Accuracy of Logistic Regression:", log_reg_accuracy, "Predictions:", log_reg_pred)

nn = NN(hidden_layer_sizes=(10,), max_iter=1000)
nn.fit(x_train, y_train)
nn_pred = nn.predict(x_test)
nn_accuracy = acc(y_test, nn_pred)
print("Accuracy of Neural Network:", nn_accuracy)