import numpy as np
mass = 22.1 / 1000 # em gramas

medidas = [-562777,-562742,-562763,-562757,-562781,-562738,-562745,-562724,-562739,-562764,-562735,-562754,-562741,-562753,]

# media = np.mean(medidas)
media = 185000
variancia = np.var(medidas)
escala = media / mass

print('Media = {:.0f}\nVariancia = {:.0f}\nEscala = {:.2f}'.format(media, variancia, escala))