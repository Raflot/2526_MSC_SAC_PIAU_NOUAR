/*
 * input_encoder.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "acquisition/input_encoder.h"

/* Définition des Pins selon ton PCB */
#define ENC_A_PIN    GPIO_PIN_6
#define ENC_A_PORT   GPIOA
#define ENC_B_PIN    GPIO_PIN_4
#define ENC_B_PORT   GPIOA
#define ENC_Z_PIN    GPIO_PIN_10  // Vérifie si c'est PC10, PC11 ou PC6 sur ton header
#define ENC_Z_PORT   GPIOC

/* Variables globales (à mettre avant le main) */
volatile int32_t encoder_position = 0; // Position absolue
volatile int32_t encoder_turns = 0;    // Nombre de tours complets (via Z)

void encoder_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Activation des horloges GPIO */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configuration ENC_A (PA6) et ENC_B (PA4) en Interruption Front Montant */
  GPIO_InitStruct.Pin = ENC_A_PIN | ENC_B_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Déclenche sur front montant
  GPIO_InitStruct.Pull = GPIO_PULLUP;         // Pull-up pour éviter le bruit
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configuration ENC_Z (PC10) */
  GPIO_InitStruct.Pin = ENC_Z_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Activation des interruptions dans le NVIC */
  /* EXTI4 pour PA4 */
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* EXTI9_5 pour PA6 (Ligne 6) */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* EXTI15_10 pour PC10 (Ligne 10) */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* Callback appelé automatiquement par HAL lors d'un front montant */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // --- Détection Front Montant sur A (PA6) ---
    if (GPIO_Pin == ENC_A_PIN)
    {
        // On lit l'état de B pour savoir le sens
        if (HAL_GPIO_ReadPin(ENC_B_PORT, ENC_B_PIN) == GPIO_PIN_RESET) {
            encoder_position++; // Sens Horaire (CW)
        } else {
            encoder_position--; // Sens Anti-Horaire (CCW)
        }
    }

    // --- Détection Front Montant sur B (PA4) ---
    else if (GPIO_Pin == ENC_B_PIN)
    {
        // On lit l'état de A
        if (HAL_GPIO_ReadPin(ENC_A_PORT, ENC_A_PIN) == GPIO_PIN_SET) {
            encoder_position++; // Sens Horaire
        } else {
            encoder_position--; // Sens Anti-Horaire
        }
    }

    // --- Détection Front Montant sur Z (Index) ---
    else if (GPIO_Pin == ENC_Z_PIN)
    {
        // Z signale un tour complet.
        // On peut soit remettre la position à 0, soit compter les tours.
        encoder_position = 0;
        encoder_turns++;
    }
}
