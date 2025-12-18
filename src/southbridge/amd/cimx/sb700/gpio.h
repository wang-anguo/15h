typedef enum {
	OUTPUT = 0,
	TRISTATE = 1
} SB700_GPIO_MODE;

typedef enum {
	ERROR = -1,
	LOW = 0,
	HIGH = 1
} SB700_GPIO_VALUE;

// Configuration
void sb700_gpio_enable(int num);
void sb700_gpio_disable(int num);
void sb700_gpio_config(int num, SB700_GPIO_MODE mode);

// Input/Output
SB700_GPIO_VALUE sb700_gpio_get(int num);
void sb700_gpio_set(int num, SB700_GPIO_VALUE value);
