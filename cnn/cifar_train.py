import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

# エポック数
MAX_EPOCH = 1

# ニューラルネットワークの定義
class CNN(nn.Module):
    def __init__(self):
         super().__init__()
         self.conv1 = nn.Conv2d(3, 6, 5)
         self.pool = nn.MaxPool2d(2, 2)
         self.conv2 = nn.Conv2d(6, 16, 5)
         self.fc1 = nn.Linear(16 * 5 * 5, 120)
         self.fc2 = nn.Linear(120, 84)
         self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
         x = self.pool(F.relu(self.conv1(x)))
         x = self.pool(F.relu(self.conv2(x)))
         x = torch.flatten(x, 1) # flatten all dimensions except batch
         x = F.relu(self.fc1(x))
         x = F.relu(self.fc2(x))
         x = self.fc3(x)
         return x

def main():

    # transform定義
    transform = transforms.Compose([transforms.Resize(32),transforms.ToTensor(), transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

    # 学習データ
    train_data_with_teacher_labels = torchvision.datasets.CIFAR10(root='./', train=True, download=True, transform=transform)
    train_data_loader = torch.utils.data.DataLoader(train_data_with_teacher_labels, batch_size=4, shuffle=True, num_workers=2)

    model = CNN()

    # optimizerの設定
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(params=model.parameters(), lr=0.001, momentum=0.9)

    # 学習
    for epoch in range(MAX_EPOCH):
        total_loss = 0.0
        for i, data in enumerate(train_data_loader, 0):
            # 学習データと教師ラベルデータを取得
            train_data, teacher_labels = data
            # 勾配情報を削除
            optimizer.zero_grad()
            # モデルで予測を計算
            outputs = model(train_data)
            # 微分計算
            loss = criterion(outputs, teacher_labels)
            loss.backward()
            # 勾配を更新
            optimizer.step()
            # 誤差
            total_loss += loss.item()
            # 1000ミニバッチずつ進捗を表示
            if i % 1000 == 999:
                print('学習進捗：[学習回数：%d, ミニバッチ数：%5d] loss: %.3f' % (epoch + 1, i + 1, total_loss / 1000))
                total_loss = 0.0

    # モデル保存
    torch.save(model.state_dict(), "model.pth")

    print("-----学習完了-----")

if __name__ == '__main__':
  main()