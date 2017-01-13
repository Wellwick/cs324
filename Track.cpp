



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
    
    
    glBegin(GL_QUADS);
	glVertex3f(track[i][0]-0.04f, track[i][1], track[i][2]-0.05f);
	glVertex3f(track[j][0]-0.038f, track[j][1], track[j][2]-0.05f);
	glVertex3f(track[j][0]+0.05f, track[j][1], track[j][2]+0.04f);
	glVertex3f(track[i][0]+0.05f, track[i][1], track[i][2]+0.038f);
    glEnd();
    
    glBegin(GL_QUADS);
	glVertex3f(track[i][0]-0.05f, track[i][1], track[i][2]-0.04f);
	glVertex3f(track[j][0]-0.05f, track[j][1], track[j][2]-0.038f);
	glVertex3f(track[j][0]+0.038f, track[j][1], track[j][2]+0.05f);
	glVertex3f(track[i][0]+0.04f, track[i][1], track[i][2]+0.05f);
    glEnd();
    */
    
    /*
    glBegin(GL_QUADS);
	glVertex3f(track[i][0]-0.05f, track[i][1], track[i][2]);
	glVertex3f(track[j][0]-0.05f, track[j][1], track[j][2]);
	glVertex3f(track[j][0]+0.05f, track[j][1], track[j][2]);
	glVertex3f(track[i][0]+0.05f, track[i][1], track[i][2]);
    glEnd();
    */
    
    
    /*
    // enable texturing
    glEnable(GL_TEXTURE_2D); 
    
    //prep the texture
    glBindTexture(GL_TEXTURE_2D, g_track);
    
    //need to calculate track length
    float dx = track[i][0] - track[j][0];
    float dy = track[i][1] - track[j][1];
    float dz = track[i][2] - track[j][2];
    float dist = sqrt((dx*dx)+(dy*dy)+(dz*dz));
    dist = dist/10.0f;
    // specify texture coordinates
    glBegin (GL_QUADS);
	glTexCoord2f (0.0f,0.0f); // lower left corner
	glVertex3f(track[i][0]-0.05f, track[i][1], track[i][2]);
	glTexCoord2f (1.0f, 0.0f); // lower right corner
	glVertex3f(track[i][0]+0.05f, track[i][1], track[i][2]);
	glTexCoord2f (1.0f, 1.0f*dist); // upper right corner
	glVertex3f(track[j][0]+0.05f, track[j][1], track[j][2]);
	glTexCoord2f (0.0f, 1.0f*dist); // upper left corner
	glVertex3f(track[j][0]-0.05f, track[j][1], track[j][2]);
    glEnd ();

    glDisable(GL_TEXTURE_2D); 
    */
}


      