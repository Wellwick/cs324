#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

/*

class Track {
    public:
	float coords[];
	float direction[];
};

//prepare the Trac
Track t1;
Track t2;
Track t3;

t1.coords = {0.3, 0.5, 0.6};
t2.coords = {-0.6, -0.2, 0.1};
t3.coords = {0.2, 0.1, -0.7};

t1.direction = t2.coords - t1.coords;
t2.direction = t3.coords - t2.coords;
t3.direction = t1.coords - t3.coords;

Track tracks[] = {t1, t2, t3};

*/

float track1[] = {0.3, 0.5, 0.6};
float track2[] = {-0.6, -0.2, 0.1};
float track3[] = {0.2, 0.1, -0.7};

// properties of some material
float mat_ambient[] = {0.05, 0.05, 0.05, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
float mat_shininess[] = {50.0};

float pos[] = {track1[0], track1[1], track1[2]};
float dir[] = {track2[0] - pos[0],
	       track2[1] - pos[1],
	       track2[2] - pos[2]};

bool g_moving = false;
bool g_coasting = false;

void idle()
{
	usleep(100000);
	//move the coaster
	pos[0] += dir[0]/50;
	pos[1] += dir[1]/50;
	pos[2] += dir[2]/50;
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	
	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (g_coasting) {
                gluLookAt(pos[0], pos[1], pos[2], // eye position
                          track2[0], track2[1], track2[2], // reference point
                          0, 1, 0  // up vector
                        );
        } else {
                gluLookAt(1, 1, 3, // eye position
                                0, 0, 0, // reference point
                                0, 1, 0  // up vector
                        );
        }

        glPushMatrix();	

                // set the surface properties
                glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

                // add in transparent geometry in front of teapot
                glDisable(GL_LIGHTING);

                glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
                glBegin(GL_QUADS);
                        glVertex3f(-1.0f, -1.0f, -1.0f);
                        glVertex3f(-1.0f, -1.0f, 1.0f);
                        glVertex3f(1.0f, -1.0f, 1.0f);
                        glVertex3f(1.0f, -1.0f, -1.0f);
                glEnd();

                glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
                glBegin(GL_LINE_LOOP);
			glVertex3f(track1[0], track1[1], track1[2]);
			glVertex3f(track2[0], track2[1], track2[2]);
			glVertex3f(track3[0], track3[1], track3[2]);
                glEnd();
                
                glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
                glBegin(GL_LINE_LOOP);
			glVertex3f(pos[0]+0.02f, pos[1]+0.02f, pos[2]);
			glVertex3f(pos[0]-0.02f, pos[1], pos[2]-0.02f);
			glVertex3f(pos[0], pos[1]-0.02f, pos[2]+0.02f);
                glEnd();
                
                
                
                glEnable(GL_LIGHTING);

        glPopMatrix();
	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(1); break;
		case 'p': pos[0] = track2[0];
			  pos[1] = track2[1];
			  pos[2] = track2[2];
			  glutPostRedisplay();
			  break;

		case ' ': g_moving = !g_moving;
				if (g_moving)
					glutIdleFunc(idle);
				else
					glutIdleFunc(NULL);
				break;
                case 'z': g_coasting = !g_coasting;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(40.0, 1.0f, 1.0, 5.0);

	glutPostRedisplay();
}

void init()
{
	float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
	float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	float light_position[] = {1.0, 1.0, 2.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH); 

	// turn on blending and set a blending function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Mixing Opaque and Transparent"); 

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 

	init(); 

	glutMainLoop(); 

	return 0; 
}
