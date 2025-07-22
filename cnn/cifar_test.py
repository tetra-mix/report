import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
from torch.nn.functional import cosine_similarity
from PIL import Image
import torch.nn.functional as F


image_path = '/content/drive/MyDrive/colab/geko.png'

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

    model = CNN()
    model.load_state_dict(torch.load("model.pth"))
    feature_extractor = torch.nn.Sequential(*(list(model.children())[:-1]))

    transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])
    preprocess = transforms.Compose([transforms.Resize(32),transforms.ToTensor(),transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

    img_org = Image.open(image_path)
    img_resize = img_org.resize((32,32))
    batch = preprocess(img_resize).unsqueeze(0)

    class_names = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')
    '''class_names = [     'apple', 'aquarium_fish', 'baby', 'bear', 'beaver', 'bed', 'bee', 'beetle',
                        'bicycle', 'bottle', 'bowl', 'boy', 'bridge', 'bus', 'butterfly', 'camel',
                        'can', 'castle', 'caterpillar', 'cattle', 'chair', 'chimpanzee', 'clock',
                        'cloud', 'cockroach', 'couch', 'crab', 'crocodile', 'cup', 'dinosaur',
                        'dolphin', 'elephant', 'flatfish', 'forest', 'fox', 'girl', 'hamster',
                        'house', 'kangaroo', 'keyboard', 'lamp', 'lawn_mower', 'leopard', 'lion',
                        'lizard', 'lobster', 'man', 'maple_tree', 'motorcycle', 'mountain', 'mouse',
                        'mushroom', 'oak_tree', 'orange', 'orchid', 'otter', 'palm_tree', 'pear',
                        'pickup_truck', 'pine_tree', 'plain', 'plate', 'poppy', 'porcupine',
                        'possum', 'rabbit', 'raccoon', 'ray', 'road', 'rocket', 'rose',
                        'sea', 'seal', 'shark', 'shrew', 'skunk', 'skyscraper', 'snail', 'snake',
                        'spider', 'squirrel', 'streetcar', 'sunflower', 'sweet_pepper', 'table',
                        'tank', 'telephone', 'television', 'tiger', 'tractor', 'train', 'trout',
                        'tulip', 'turtle', 'wardrobe', 'whale', 'willow_tree', 'wolf', 'woman',
                        'worm'
                        ]'''

    class_corrent = list(0. for i in range(10))
    class_total = list(0. for i in range(10))

    prediction = model(batch).squeeze(0).softmax(0)
    class_id = prediction.argmax().item()
    score = prediction[class_id].item()
    #print(class_id,score)
    #print(class_names[class_id],score)
    print(prediction)

if __name__ == '__main__':
  main()