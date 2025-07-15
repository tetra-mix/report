import sklearn
import joblib
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.tree import plot_tree

def main():
  read_data_name = '/content/drive/MyDrive/colab/num3_o.csv' #読み込むCSVファイル名指定
  save_model_name ='/content/drive/MyDrive/colab/num3_o.pkl' #保存する学習モデル名指定
  save_figu_name = '/content/drive/MyDrive/colab/exp1.png' #保存する画像名指定

  dfs = pd.read_csv(read_data_name)
  print(dfs['Nxt'])
  X = dfs.drop(['Nxt'],axis=1)
  Y = dfs['Nxt']
  x_train, x_test, y_train, y_test = \
  train_test_split(X, Y, test_size=0.3, random_state=1)

  #forest = RandomForestClassifier(criterion='entropy',n_jobs=-1,max_depth=100,n_estimators=100,bootstrap=True)
  #forest = RandomForestClassifier(criterion='gini',n_jobs=-1,max_depth=100,n_estimators=100,bootstrap=True)
  #forest = RandomForestClassifier(criterion='log_loss',n_jobs=-1,max_depth=100,n_estimators=100,bootstrap=True)
  forest = RandomForestClassifier()
  forest.fit(x_train, y_train)
  joblib.dump(forest,save_model_name,compress=True)
  print('Train score: {}'.format(forest.score(x_train, y_train)))
  print('Test score: {}'.format(forest.score(x_test, y_test)))

  feature_names = [f'Feature {i+1}' for i in range(x_train.shape[1])]
  feature_names = X.columns.tolist()

  plt.figure(figsize=(100, 60))
  plot_tree(forest.estimators_[0],filled=True,feature_names=feature_names,max_depth=4);
  plt.show()
  plt.savefig(save_figu_name)

if __name__ == '__main__':
  main()