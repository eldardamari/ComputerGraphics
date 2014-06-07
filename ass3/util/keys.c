void mouse(int button, int state, int x, int y) 
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			LeftClick   =   true;
			RightClick  =   false;
			MiddleClick =   false;

			if (state == GLUT_DOWN)
			{
				if (GlobalMode){
					xdiff = x - yrot;
					ydiff = -y + xrot;
				}
				if (CameraMode){
					camrotx=x;
					camroty=y;
				}
				if (PickingMode){
					xxPick=x;
					yyPick=y;
					mode = SELECT;
				}
			}
			break;
			case GLUT_MIDDLE_BUTTON:
				LeftClick=false;
				RightClick=false;
				MiddleClick=true;
				if (state == GLUT_DOWN){
					if (GlobalMode){
						movex=x;
						movey=y;
					}
					if (CameraMode){
						movecamx=x;
					}
					if (PickingMode){
						xxPick=x;
						yyPick=y;
						mode = SELECT;
						
					}
				}
			break;
			case GLUT_RIGHT_BUTTON:
				LeftClick=false;
				RightClick=true;
				MiddleClick=false;
				if (state == GLUT_DOWN){
					if (GlobalMode){
						scaley=y;
					}
					if (CameraMode){
						movecamz=y;
					}
					if (PickingMode){
						xxPick=x;
						yyPick=y;
						mode = SELECT;
					}
				}
			break;
			}
}

void motion(int x,int y)
{
		if (GlobalMode){
			if (LeftClick){
					yrot = x - xdiff;
					xrot = y + ydiff;
			}
			if (MiddleClick){
					tranx = tranx - (movex - x)/10.0;
					trany = trany + (movey - y)/10.0;
					movex=x;
					movey=y;
			}
			if (RightClick){
				rescaley = rescaley + (scaley - y)/128.0;
				scaley=y;
			}
		}//GlobalMode
		if (CameraMode){
			if (LeftClick){
				camrotxdiff = camrotxdiff + (camrotx-x)/5.0;
				camrotydiff = camrotydiff - (camroty-y)/5.0;
				
				camrotx=x;
				camroty=y;
	
			}
			if (MiddleClick){
					camtranx = camtranx - (movecamx - x) /10.0;
						movecamx=x;
			}
			if (RightClick){
				camtranz = camtranz + (movecamz - y)/10.0;
				movecamz=y;
			}
		}
		if (PickingMode){
			if (LeftClick){
				if (Pickindex >-1 ){
				PickRotX.at(Pickindex) -= (yyPick-y)/10.0;
				PickRotY.at(Pickindex) -= (xxPick-x)/10.0;
				}
				xxPick=x;
				yyPick=y;
			
			}
			if (RightClick){
				if (Pickindex > -1 ){
				PickScale.at(Pickindex)+=(yyPick-y)/60.0;
				}
				xxPick=x;
				yyPick=y;
			
			}
			if (MiddleClick){
				if (Pickindex > -1 ){
				PickTransX.at(Pickindex)-=(xxPick-x)/10.0;
				PickTransY.at(Pickindex)+=(yyPick-y)/10.0;
				}
				xxPick=x;
				yyPick=y;
			}
		
		}
	
	glutPostRedisplay();
}
