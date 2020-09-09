import torch.nn as nn

class Classifier(nn.Module):

    def __init__(self, model, input_dim=64, output_dim=3, n_features=16):
        super(Classifier, self).__init__()
        
        # feature extractor
        self.features = nn.Sequential(
            *list(model.features.children())[:-1],
            nn.AvgPool2d(input_dim // 8, 1),
        )

        # linear classifier
        self.classifier = nn.Sequential(
            nn.Linear(n_features, output_dim),
        )

        self.n_features = n_features

    def forward(self, x):
        x = self.features(x)
        x = x.view(-1, self.n_features)
        return self.classifier(x)