# Woody-Wood Packer

## Plan

1. Get infos:
   1. choper la zone a crypter
   2. Trouver la zone ou s'inserer
   3. Recuperer notre code a injecter
2. Do things
    1. Entry Point
       1. Obtenir le true entry point: Soustraire l'entry point a l'adresse virtuelle
       2. Changer le jump final dans la zone que l'on insere vers l'ancien entry point
    2. Crypter la section .text
    3. Ajouter notre packer