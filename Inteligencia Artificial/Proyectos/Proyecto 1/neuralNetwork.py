import numpy as np
import pandas as pd
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.neighbors import KNeighborsClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score
from imblearn.over_sampling import SMOTE
dataset = pd.read_csv('Datasets/diabetes.csv')
X = dataset.drop("Outcome", axis=1)  # Features
y = dataset["Outcome"]  # Labels

x_train, x_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
x_train, y_train = SMOTE(random_state=42).fit_resample(x_train, y_train)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(x_train)
X_test_scaled = scaler.transform(x_test)
accuracy = {}
for i in range(1, 201): #Probando neuronas [1,200]
    nn = MLPClassifier(hidden_layer_sizes=(i,), max_iter=10000)
    nn.fit(X_train_scaled, y_train)
    nn_pred = nn.predict(X_test_scaled)
    nn_accuracy = accuracy_score(y_test, nn_pred)
    recall = recall_score(y_test, nn_pred)
    precision = precision_score(y_test, nn_pred)
    print("Accuracy of Neural Network:", nn_accuracy)
    print("Recall of Neural Network:", recall)
    print("Precision of Neural Network:", precision)
    accuracy[i] = nn_accuracy
with open('accuracy.txt', 'w') as f:
    print(accuracy, file=f)
