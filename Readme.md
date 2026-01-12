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

L'un des problèmes qu'on peut rencontrer est une motée soudaine et extrème du courant. En effet, sur une très courte période le courant peut atteindre les 70 Ampères, ce qui est bien au dessus du courant nominal du moteur.

On cherche maintenant à mesurer le courant en sortie du transistor qui commande le moteur. 

Dans le fichier schéma global sur Kicad, on peut observer que le chopper Arm_IRF540NBbF contient un pin Imes qui correspond à la mesure de courannt. On peut voir que ce pin est relié au pin PA1 de la carte Nucléo, donc on fera notre mesure de courant sur ce pin là




<p align="center">
  <img width="473" height="419" alt="Capture d&#39;écran 2025-12-10 165612" src="https://github.com/user-attachments/assets/87946e84-b1a0-4522-a07b-7c14b5cc1b03" />
</p>

<p align="center">
  <img width="299" height="294" alt="image" src="https://github.com/user-attachments/assets/23ad4e0c-57f3-472b-80ef-3e259fa69557" />
</p>

<p align="center">
  <img width="403" height="391" alt="image" src="https://github.com/user-attachments/assets/b2ea57ee-55b2-409f-9b1e-ca1ffbc02978" />
</p>

<p align="center">
  <img width="630" height="565" alt="image" src="https://github.com/user-attachments/assets/ee9febae-fedc-4cf4-a684-a8253753b650" />
</p>

En cherchant dans la datasheet du capteur GO 10-SME/SP3, on peut retrouver la fonction de transfert du capteur de mesure de courant : U<sub>out</sub> = U<sub>ref</sub> + (S<sub>N</sub> * I<sub>P</sub>)

Avec :

U<sub>ref</sub> : la tension de référence du capteur, égale à 1.65 V

S<sub>N</sub> : la sensibilité nominale du capteur, égale à 50 mV/A 

I<sub>P</sub> : le courant mesuré par le capteur, compris entre -25 A et 25 A

Apres avoir testé la mesure de courant en pooling on configure l'adc, le dma et le timer 1 pour mesurer le courant en phase avec la pwm.

on obtient apres calibration une mesure plutot fiable : (attendue 1.3v-1.4v a 70 et 0v a 50) 

<p align="center">
  <img width="600" height="475" alt="image" src="https://github.com/user-attachments/assets/dd5edea7-e94b-4845-ba40-45999547be6c" />
</p>

