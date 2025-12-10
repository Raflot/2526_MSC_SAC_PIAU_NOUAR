Swann PIAU

Mahdi NOUAR

# Compte rendu TP MSC système d'acquisition et de commande 

La première chose qu'on fait est tester le shell de la stm32 pour vérifer son bon fonctionnement :



<p align="center">
  <img width="413" height="200" src="https://github.com/user-attachments/assets/217dc32c-d0b1-4006-b4db-e705c98da8e3" alt="Sublime's custom image"/>
</p>



Ensuite, on configure la stm32 pour pouvoir créer des PWM :



<p align="center">
  <img width="1593" height="772" alt="image" src="https://github.com/user-attachments/assets/b6eaf50d-8a77-4558-acd5-122ead31d4c4" />
</p>



On cherche à avoir une PWM de fréquence 20 kHZ. 

Le Timer utilisé est le Timer 1, dont la clock a une fréquence de 170 MHz.

Il faut donc régler le PSC et l'ARR aifn d'avoir la fréquence de PWM désirée

Or, on a 

f<sub>PWM</sub> = f<sub>Timer</sub> / (1 + ARR)(1 + PSC)

avec :

f<sub>PWM</sub> = 20 kHZ

f<sub>Timer</sub> = 170 MHz

Il faut donc que (1 + ARR)(1 + PSC) = f<sub>Timer</sub> / f<sub>PWM</sub> = 8500

D'autre part, l'ARR est codé sur 16 bits, donc il peut prendre toutes les valeurs entre 0 et 65535. Et comme on privilégie un grand ARR, on pose PSC = 0 et ARR = 8499

On cherche de plus a avoir une resolution de minimum 10 Bits or Log²(8499) = 13 Bits > 10.

Enfin on regarde la Datasheet des transistors afin de mettre en place un temps mort sufisant, on a :



<p align="center">
  <img width="724" height="80" alt="image" src="https://github.com/user-attachments/assets/c7e10077-89d7-4601-825d-a322e668f5b1" />
</p>



il nous faut donc un deadtime > 39 (rise time) + 50 (securitée) = 89 ns 

Or notre carte etant a une frequence de 170 MHz un tick vaut ~ 1/170 MHz = 5.88 ns. Il nous faut donc un dead time de 16 ticks. (5.88 * 16 = 94)

On peut observer ce dead time en zoomant sur l'axe temporel entre le front descendant d'une PWM envoyé sur le channel 1 et sa complémentaire :



<p align="center">
  <img width="800" height="480" alt="tek00002" src="https://github.com/user-attachments/assets/1b089839-8ffe-430d-9935-444f624d22ef" />
</p>



On écrit le code suivant dans le fichier main.c du projet : 



<p align="center">
  <img width="1000" height="520" alt="image" src="https://github.com/user-attachments/assets/8c5a7369-17ea-4e64-bdcc-6f00dc025a03" />
</p>



En compilant le code précédent, on obtient alors ce signal obsérvé à l'oscilloscope



<p align="center">
  <img width="800" height="480" alt="tek00000" src="https://github.com/user-attachments/assets/040046fe-fafb-42cd-a370-3efe8779d2d9" />
</p>



On désactive le channel 2, et on met le rapport cyclique du channel 1 à 50% :



<p align="center">
  <img width="800" height="480" alt="tek00001" src="https://github.com/user-attachments/assets/706c6b75-9157-4c5a-9082-50f8e4aa0e29" />
</p>



On remet le channel 2 et on le configure pour envoyer des PWMs en complémentaire décallé avec celles du channel 1 pour pouvoir commander le moteur 

On envoie une tension de 6 V et on limite le courant envoyé au moteur à 1 A : on peut alors observer les signaux sur l'oscilloscope :



<p align="center">
  <img width="800" height="480" alt="tek00003" src="https://github.com/user-attachments/assets/ea17e963-5309-4b52-ab9a-b3ef07173008" />
</p>



On peut observer en jaune la tension du moteur, 
