import pygame
import os
pygame.init()
if os.path.exists('bbc_churches--_07054115.mp3'):
  pygame.mixer.music.load('bbc_churches--_07054115.mp3')
  pygame.mixer.music.play()
  pygame.mixer.music.set_volume(1)

  clock = pygame.time.Clock()
  clock.tick(3)

  while pygame.mixer.music.get_busy():
     pygame.event.poll()
     clock.tick(3)
else:
  print('O arquivo musica.mp3 não está no diretório do script Python')