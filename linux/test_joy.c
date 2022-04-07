#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

  typedef struct {
    __s16 axis[4];
    __s16 button[16];
  } js_status;

main() {
  int a, b, x;
  char number_of_axes, number_of_buttons;
  struct js_event e;
  int *fd[4];
  js_status joypad[4];
  char *direction;

  (int)fd[0] = open ("/dev/js0", O_NONBLOCK);

  ioctl ((int)fd[0], JSIOCGAXES, &number_of_axes);
  ioctl ((int)fd[0], JSIOCGBUTTONS, &number_of_buttons);

  printf("Number of Axes on JS0: %d\n", number_of_axes);
  printf("Number of Buttons on JS0: %d\n", number_of_buttons);

  while (1) {
    int joy_event = 0;
    while (read ((int)fd[0], &e, sizeof(e)) > 0) {
      if (e.type & JS_EVENT_BUTTON) {
	joypad[0].button[e.number] = e.value;
	joy_event = 1;
      } else if (e.type & JS_EVENT_AXIS) {
	joypad[0].axis[e.number] = e.value;
	joy_event = 1;
      }
    }

    // Print joystick status if it's changed...
    if (joy_event == 1) {
      printf("Direction:\t");
      // for (x=0; x < (int)number_of_axes; x++) {
      //   printf("%d\t",joypad.axis[x]);
      // }

      strcpy(direction,"");
      if (joypad[0].axis[0] == 0 && joypad[0].axis[1] == -32767) {
	      strcpy(direction,"Up");
      } else if (joypad[0].axis[0] == 32767 && joypad[0].axis[1] == -32767) {
	      strcpy(direction,"Upper Right");
      } else if (joypad[0].axis[0] == 32767 && joypad[0].axis[1] == 0) {
	      strcpy(direction,"Right");
      } else if (joypad[0].axis[0] == 32767 && joypad[0].axis[1] == 32767) {
	      strcpy(direction,"Lower Right");
      } else if (joypad[0].axis[0] == 0 && joypad[0].axis[1] == 32767) {
	      strcpy(direction,"Down");
      } else if (joypad[0].axis[0] == -32767 && joypad[0].axis[1] == 32767) {
	      strcpy(direction,"Lower Left");
      } else if (joypad[0].axis[0] == -32767 && joypad[0].axis[1] == 0) {
	      strcpy(direction,"Left");
      } else if (joypad[0].axis[0] == -32767 && joypad[0].axis[1] == -32767) {
	      strcpy(direction,"Upper Left");
      }

      printf("%s",direction);

      printf("\nButtons:\t");
      for (x=0; x < (int)number_of_buttons; x++) {
        printf("%d:%d  ",x,joypad[0].button[x]);
      }
      
      printf("\n\n");
    }
    usleep(1);
  }

  close ((int)fd[0]);
}
