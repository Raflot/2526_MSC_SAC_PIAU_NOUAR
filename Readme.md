Swann PIAU

Mahdi NOUAR

# Compte rendu TP MSC système d'acquisition et de commande 

On teste le shell de la stm32 pour vérifer son bon fonctionnement :

![Sans titre](https://github.com/user-attachments/assets/217dc32c-d0b1-4006-b4db-e705c98da8e3)

On cherche à avoir une PWM de fréquence 20 kHZ. 

Le Timer utilisé est le Timer 1, dont la clock a une fréquence de 170 MHz.

Il faut donc régler le PSC et l'ARR aifn d'avoir la fréquence de PWM désirée

Or, on a 

f<sub>PWM</sub> = f<sub>Timer</sub> /(1 + ARR)(1 + PSC)

avec :

f<sub>PWM</sub> = 20 kHZ

f<sub>Timer</sub> = 170 MHz

Il faut donc que (1 + ARR)(1 + PSC) = 8500

D'autre part, l'ARR est codé sur 16 bits, donc il peut prendre toutes les valeurs entre 0 et 65535. Et comme on privilégie un grand ARR, on pose PSC = 0 et ARR = 8499

On cherche de plus a avoir une resolution de minimum 10 Bits or Log²(8499) = 13 Bits > 10.

Enfin on regarde la Datasheet des transistors afin de mettre en place un temps mort sufisant, on a :

<img width="724" height="80" alt="image" src="https://github.com/user-attachments/assets/c7e10077-89d7-4601-825d-a322e668f5b1" />

il nous faut donc un deadtime > 39 (rise time) + 50 (securitée) = 89 ns 

Or notre carte etant a une frequence de 170 MHz un tick vaut ~ 1/170 MHz = 5.88 ns. Il nous faut donc un dead time de 16 ticks. (5.88 * 16 = 94)
