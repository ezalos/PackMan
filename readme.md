# Woody-Wood Packer

This goal of this program is to code a program which will have the task to encrypt a program passed as a parameter. 

A new "woody" program will then be generated at the end of the execution of the program. When this new program (woody) is executed, it will have to be deciphered
to be able to get started. Its execution will be identical in all respects with the first program passed as a parameter in the previous step.

## Features

Formats supported:
* ELF64
* ELF

## Run

If you're not on a Linux machine:
```
docker build -t woody .
docker run -ti -v "$(pwd)":/woody_woodpacker woody
cd woody_woodpacker
```

You have two possible ways to launch the program:

```
make run [FILE_TO_CRYPT]
```

or:

```
make
./woody [FILE_TO_CRYPT]
```

## Walkthrough

1. Parse the ELF

2. Find zones to crypt and caves

3. Different injections methods

   1. 
   2.
   3.

## Plan

1. Get infos:LIB
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