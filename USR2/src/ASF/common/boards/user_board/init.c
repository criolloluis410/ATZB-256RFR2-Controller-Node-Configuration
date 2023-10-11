#include <asf.h>
#include <board.h>



#include <sio2host.h>


/**
 * \brief Initializes the board components
 */
void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	 // INICIALIZACION DE PUERTOS
	 //Para el pulsador.
	 ioport_configure_pin(GPIO_PUSH_BUTTON_0,IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	 //Para diodos led.
	 ioport_configure_pin(LED0, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	 ioport_configure_pin(LED1, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	 ioport_configure_pin(LED2, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
}

/**
 * \brief Initializes the modules added by the Project Wizard
 */
void modules_init(void)
{
	sio2host_init();

}
