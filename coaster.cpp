#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>

//create multi-dimensional array for the track
float track[][3] = {{0.3, 0.5, 0.6},
		   {-0.6, -0.2, 0.1},
		   {-0.7, 0.4, 0.3},
		   {0.2, 0.1, -0.7}};
int trackCounter = 1; //counts which index in the track is next

// properties of some material
float mat_ambient[] = {0.05, 0.05, 0.05, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
float mat_shininess[] = {50.0};

float pos[] = {track[0][0], track[0][1], track[0][2]};
float dir[] = {(track[1][0] - pos[0])/50,
	       (track[1][1] - pos[1])/50,
	       (track[1][2] - pos[2])/50};
float speed = 1.0f;

bool g_moving = false;
bool g_coasting = false;

void setNextTrack() {
    trackCounter = (trackCounter + 1) % (sizeof(track)/sizeof(track[0]));
    dir[0] = track[trackCounter][0] - pos[0];
    dir[1] = track[trackCounter][1] - pos[1];
    dir[2] = track[trackCounter][2] - pos[2];
    //normalise the direction
    float absoluteVal = (dir[0]*dir[0]) + (dir[1]*dir[1]) + (dir[2]*dir[2]);
    absoluteVal = sqrt(absoluteVal);
    //scale to a fiftieth of the speed
    dir[0] = dir[0]/(absoluteVal*50);
    dir[1] = dir[1]/(absoluteVal*50);
    dir[2] = dir[2]/(absoluteVal*50);
    /*
    float speed = sqrt((dir[0]*dir[0]) + (dir[1]*dir[1]) + (dir[2]*dir[2]));
    std::cout << "Speed for next section will be " << speed << std::endl;
    */
}

void idle()
{
    usleep(100000);
    //move the coaster
    pos[0] += dir[0]*speed;
    pos[1] += dir[1]*speed;
    pos[2] += dir[2]*speed;
    
    //change speed based on how much we should accelerate
    float dist = sqrt((dir[0]*dir[0]) + (dir[2]*dir[2]));
    float angle = atan(dir[1]/dist);
    float acceleration = (-sin(angle)*9.81)/60; 
    if (angle < 0.0f) acceleration - 0.05f;
    speed += acceleration;
    if (speed < 1.0f) speed = 1.0f; //always maintain minimum speed
    //std::cout << "Speed is currently " << speed << std::endl;
    
    //make sure we haven't overshot the destination
    float tmp[] = {pos[0]+dir[0], pos[1]+dir[1], pos[2]+dir[2]};
    //calculate square of distance to next track point
    float dx = pos[0] - track[trackCounter][0];
    float dy = pos[1] - track[trackCounter][1];
    float dz = pos[2] - track[trackCounter][2];
    float calcNow = (dx*dx) + (dy*dy) + (dz*dz);
    
    //calculate what it will be by next tick
    dx = tmp[0] - track[trackCounter][0];
    dy = tmp[1] - track[trackCounter][1];
    dz = tmp[2] - track[trackCounter][2];
    float calcNext = (dx*dx) + (dy*dy) + (dz*dz);
    
    //if we are going to be further away, just snap to the next point
    if (calcNext > calcNow) {
	pos[0] = track[trackCounter][0];
	pos[1] = track[trackCounter][1];
	pos[2] = track[trackCounter][2];
	setNextTrack();
    }
    
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
    
    // position and orient camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (g_coasting) {
	gluLookAt(pos[0], pos[1]+0.05f, pos[2], // eye position
		  pos[0]+dir[0], pos[1]+0.05f+dir[1], pos[2]+dir[2], // reference point
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
	glBegin(GL_QUADS);
	    glVertex3f(-0.8f, -0.9f, -1.0f);
	    glVertex3f(-0.6f, -0.9f, -1.0f);
	    glVertex3f(1.0f, -0.9f, 0.6f);
	    glVertex3f(1.0f, -0.9f, 0.8f);
	glEnd();
	glBegin(GL_QUADS);
	    glVertex3f(-1.0f, -0.9f, -0.8f);
	    glVertex3f(-1.0f, -0.9f, -0.6f);
	    glVertex3f(0.6f, -0.9f, 1.0f);
	    glVertex3f(0.8f, -0.9f, 1.0f);
	glEnd();
	
	
	//draw track
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	int maxSize = sizeof(track)/sizeof(track[0]);
	for (unsigned int i=0; i < maxSize; i++) {
	    float tWid = 0.05f; //half of the track width
	    float rWid = 0.025f; //half of the rail width
	    int j = (i+1)%maxSize;
	    /*
	    for (int k=-1; k<3; k+=2) {
		//left track
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]+rWid, track[i][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]+rWid, track[i][2]);
		glEnd();
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]-rWid, track[i][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]-rWid, track[i][2]);
		glEnd();
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]-rWid, track[i][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]+rWid, track[i][2]);
		glEnd();
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]-rWid, track[i][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]+rWid, track[i][2]);
		glEnd();
	    }
	    */
	    
	    glBegin(GL_QUADS);
		glVertex3f(track[i][0]-0.04f, track[i][1], track[i][2]-0.05f);
		glVertex3f(track[j][0]-0.03f, track[j][1], track[j][2]-0.05f);
		glVertex3f(track[j][0]+0.05f, track[j][1], track[j][2]+0.04f);
		glVertex3f(track[i][0]+0.05f, track[i][1], track[i][2]+0.03f);
	    glEnd();
	    
	    glBegin(GL_QUADS);
		glVertex3f(track[i][0]-0.05f, track[i][1], track[i][2]-0.04f);
		glVertex3f(track[j][0]-0.05f, track[j][1], track[j][2]-0.03f);
		glVertex3f(track[j][0]+0.03f, track[j][1], track[j][2]+0.05f);
		glVertex3f(track[i][0]+0.04f, track[i][1], track[i][2]+0.05f);
	    glEnd();
	    
	}
	
	
	
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	/*
	glBegin(GL_LINE_LOOP);
	    for (unsigned int i=0; i < sizeof(track)/sizeof(track[0]); i++) {
		//need to figure out spot for left and right rail
		//just shifting along the x dimension
		
		
		glVertex3f(track[i][0], track[i][1], track[i][2]);
	    }
	glEnd();
	*/
	
	//draw the cart
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
	case 'p': pos[0] = track[trackCounter][0];
		  pos[1] = track[trackCounter][1];
		  pos[2] = track[trackCounter][2];
		  setNextTrack();
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
