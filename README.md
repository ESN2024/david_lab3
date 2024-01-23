# Introduction
L'objectif de ce TP est d'aller encore plus loin en termes de complexité dans la conception conjointe d'un système sur carte de développement FPGA.

Nous allons donc reprendre le système que nous avons conçu lors du TP2, et nous allons cette fois-ci y implémenter un capteur d'accélération ainsi qu'un bus I2C qui permettra la communication entre le système présent et le capteur.

Nous aimerions à l'issue du TP pouvoir acquérir des mesures d'accélérations sur les trois axes préalablement calibrés. Nous voulons pouvoir afficher ces valeurs sur les afficheurs 7-segments et nous voulons enfin utiliser un bouton-poussoir pour faire la sélection de l'axe de mesure que l'on souhaite afficher. 
# 1. Description du système
La figure 1 comporte le synoptique du système.
![image](https://github.com/ESN2024/david_lab3/assets/124572489/28ca04ff-5296-4cab-aa06-8882ab311a21)
*Figure 1. Synoptique du système.*

Nous voyons l'ajout de l'accéléromètre. Se trouve aussi sur l'outil Qsys un bloc IP libre appelé "opencores I2C" qui est un convertisseur I2C vers Avalon Memory-Mapped et inversement. Ce bloc nous permet de communiquer et recevoir des informations avec le capteur.



Une grande partie du travail sera donc de mettre en place la communication entre notre système et le catpeur à partir de la documentation de celui-ci. Nous allons écrire des fonctions en C à partir des fonctions de communication I2C fournies par la librairie HAL. 

Une seconde partie est la calibration du capteur et le conditionnement du signal binaire signé renvoyé par le capteur en signal affichable sur 7-segment.

Enfin, la dernière partie qui est réalisée est 

La figure 2 présente la configuration de l'outil *Platform Designer* utilisé pour répondre au cahier des charges.
![image](https://github.com/ESN2024/david_lab3/assets/124572489/e3582c0d-30d6-492f-b495-ed8adf422208)
*Figure 2. Configuration Qsys.*
# 2. Résultats
Le système final correspond bien aux attentes demandées, voir vidéo 1 présentant son fonctionnement.

*Vidéo 1. Système en fonctionnement.*
La fonction de mesure a 
# Conclusion

