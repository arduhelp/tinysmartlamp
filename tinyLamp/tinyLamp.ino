#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB3      // Пін для підключення світлодіода
#define BUTTON_PIN PB4   // Пін для підключення кнопки

// Функція для перевірки стану кнопки з антидребезгом
bool isButtonPressed() {
    static uint8_t lastState = 0xFF;
    static uint8_t debounceCounter = 0;
    uint8_t currentState = (PINB & (1 << BUTTON_PIN)) ? 1 : 0;

    if (currentState == lastState) {
        if (debounceCounter < 5) debounceCounter++;
    } else {
        debounceCounter = 0;
    }

    lastState = currentState;
    return (debounceCounter >= 5 && currentState == 0); // 0 - кнопка натиснута
}

int main() {
    // Налаштування
    DDRB |= (1 << LED_PIN);   // Налаштувати LED_PIN як вихід
    DDRB &= ~(1 << BUTTON_PIN); // Налаштувати BUTTON_PIN як вхід
    PORTB |= (1 << BUTTON_PIN); // Вмикаємо внутрішній підтягуючий резистор для кнопки

    bool ledState = false; // Початковий стан світлодіода

    while (1) {
        if (isButtonPressed()) {
            _delay_ms(20); // Захист від повторних спрацьовувань
            ledState = !ledState; // Перемикання стану світлодіода

            if (ledState) {
                PORTB |= (1 << LED_PIN); // Увімкнути світлодіод
            } else {
                PORTB &= ~(1 << LED_PIN); // Вимкнути світлодіод
            }

            // Невелика затримка для уникнення повторного спрацьовування
            _delay_ms(200);
        }
    }

    return 0;
}
