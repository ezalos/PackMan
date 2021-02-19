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

1. Check header (elf ? x64 ? executable ? etc.)
2. Make binary tree:
   1. segments bt contain section bt (Is in a holding struct)
   2. solve available space after in holding struct (can be done just for segement)
3. Find loadable segments
   1. crypt them and keep track of their associated zones
4. find loadable + executable zones
   1. Solve where to insert code  if possible, or print error.
   2. insert the code
      1. entry point
      2. uncrypt
      3. write woody
      4. jump to hold entry point
   3. Chang segment + section size to include injection
5. Save the woody