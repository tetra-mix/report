import sklearn
import joblib
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.tree import plot_tree 

def main():

  dfs = pd.read_csv('/content/drive/MyDrive/colab/num3_n.csv')
  X = dfs

  loaded_rf_model = joblib.load(open("/content/drive/MyDrive/colab/num3_h.pkl","rb"))
  predict = loaded_rf_model.predict(X)
  print("{}".format(predict))
  loaded_rf_model = joblib.load(open("/content/drive/MyDrive/colab/num3_t.pkl","rb"))
  predict = loaded_rf_model.predict(X)
  print("{}".format(predict))
  loaded_rf_model = joblib.load(open("/content/drive/MyDrive/colab/num3_o.pkl","rb"))
  predict = loaded_rf_model.predict(X)
  print("{}".format(predict))

if __name__ == '__main__':
  main()