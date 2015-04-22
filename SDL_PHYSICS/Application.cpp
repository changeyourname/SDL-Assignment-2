#include "Application.h"

/// 
/// Application.cpp
/// SDL Assignment 2
/// This class contains variables and data to handle the update and drawing of an OpenGL executable.
/// Created by Joshua Cook -- 2015
/// 

Application::Application(int WIDTH, int HEIGHT)
{
	winWidth = WIDTH;
	winHeight = HEIGHT;
}

Application::~Application(void)
{
}

/// Initialisation of Application.
/// Includes the startup of SDL and OpenGL.
bool Application::init()
{
	// first init SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "Could not initialise SDL" << std::endl;
		return false;
	}

	// then tell SDL we are using OpenGL	
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// create a window
	window = SDL_CreateWindow("Joshua Cook SDL", 100, 100, winWidth, winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// create the buffer
	renderer = SDL_CreateRenderer(window, -1, 0);

	// create OpenGL Context thing...
	glcontext = SDL_GL_CreateContext(window);

	// then load GLEW
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) <<std::endl;
		return false;
	}

	// spit out GLEW + OpenGL info
	std::cout<<"INFO: Using GLEW "<< glewGetString(GLEW_VERSION)<<std::endl;

	std::cout << "INFO: OpenGL Vendor: " << glGetString( GL_VENDOR ) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString( GL_RENDERER ) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString( GL_VERSION ) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

	// features of OpenGL we wish to use
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// and any extras for SDL
	SDL_ShowCursor(false);

	running = true;

	initObjects();

	return true;
}

/// Initialisation of Objects.
/// Spserates the creation of the window and creation of GameObjects.
void Application::initObjects()
{
	input = new SDL_Input();

	/// Load in the table mesh and textures
	gameTable = new Table();

	Mesh* table = new Mesh("Models/Table.obj");
	table->LoadTexture("Textures/Wood1.bmp");

	Mesh* playArea = new Mesh("Models/PlayArea.obj");
	playArea->LoadTexture("Textures/Cloth.bmp");

	gameTable->Create(playArea, table);
	

	Mesh* ballMesh = new Mesh("Models/Ball.obj");
	ballMesh->LoadTexture("Textures/CueBall.bmp");

	/// Create the vector of game balls
	for(unsigned int i = 0; i < 16; i++)
	{
		Ball* newBall = new Ball();

		newBall->AttachMesh(*ballMesh);
		//newBall->SetPosition(x, 10 - y, z);

		newBall->Kick(glm::vec3(rand() % 10, rand() % 10, rand() % 10));

		balls.push_back(newBall);
	}


	camera = new Camera();
}

/// Frame Step of Application.
/// Includes 'most' of the computation for the applications entities.
void Application::Update(float dt)
{
	// reset buffer
	glClearColor(0.0f,0.0f,0.3f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	input->Update();
	
	camera->Update(input, window, dt);
	camera->Orbit(glm::vec3(0, 0, 0), 5);

	unsigned int ballSize = balls.size();
	for(int i = 0; i < ballSize; i++)
	{
		balls[i]->Update(balls, dt);
		//balls[i]->Orbit(glm::vec3(0, 0, 0), dt);
		//std::cout << balls[i]->getPosition().x << ", " << balls[i]->getPosition().y << ", " << balls[i]->getPosition().z << std::endl;
	}

}

/// Draw Step of Application.
/// Includes the drawing of entities in the application.
void Application::Draw()
{	

	// draw objects using cameras perspective
	gameTable->Draw(camera->getViewMatrix(), camera->getProjectionMatrix());
	
	unsigned int ballSize = balls.size();
	for(int i = 0; i < ballSize; i++)
	{
		balls[i]->Draw(camera->getViewMatrix(), camera->getProjectionMatrix());
	}		

	// do the render
	SDL_GL_SwapWindow(window);
}

/// Game Loop.
/// Not much to say here, constantly goes over Update and Draw functions
void Application::run()
{	
	unsigned int lastTime = SDL_GetTicks();

	while(running)
	{
		unsigned int current = SDL_GetTicks();
		float dt = (float) (current - lastTime) / 1000.0f;
		lastTime = current;
		
		if( dt < (1.0f/60.0f) )
		{
			SDL_Delay((unsigned int) (((1.0f/60.0f) - dt) * 1000.0f));
		}

		Update(dt);
		Draw();

		if(input->Esc())
			running = false;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(glcontext);
}