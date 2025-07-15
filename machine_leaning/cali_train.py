import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.datasets import fetch_california_housing
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score

cali_housing = fetch_california_housing(as_frame=True)
# 説明変数
df = pd.DataFrame(cali_housing.data, columns=cali_housing.feature_names)
# 目的変数
df['Price'] = cali_housing.target
display(df.head())
# ヒートマップ作成
plt.figure(figsize=(12, 9))
sns.heatmap(df.corr(), annot=True, cmap='Blues', fmt='.2f', linewidths=.5)
plt.savefig('california_housing_heatmap.png')
# トレーニングデータ作成
X = df.drop('Price', axis=1)
y = df['Price']
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 線形回帰モデルを作成
model = LinearRegression()
# モデルをトレーニングデータにフィット
model.fit(X_train, y_train)
# テストデータを用いて予測
y_pred = model.predict(X_test)

# スコア計算と表示
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)
print(f"Mean Squared Error: {mse}")
print(f"R^2 Score: {r2}")

# 回帰係数を表示
coefficients = pd.DataFrame(model.coef_, X.columns, columns=['Coefficient'])
print(coefficients)