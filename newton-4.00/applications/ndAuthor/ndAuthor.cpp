/* Copyright (c) <2003-2022> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#include "ndAuthorStdafx.h"
#include "ndAuthor.h"
//#include "ndSkyBox.h"
//#include "ndDemoMesh.h"
//#include "ndVehicleUI.h"
//#include "ndDemoEntity.h"
//#include "ndDemoCamera.h"
//#include "ndShaderCache.h"
//#include "ndLoadFbxMesh.h"
//#include "ndFileBrowser.h"
//#include "ndDebugDisplay.h"
//#include "ndPhysicsWorld.h"
//#include "ndPhysicsUtils.h"
//#include "ndDebugDisplay.h"
//#include "ndTestDeepBrain.h"
//#include "ndDemoDebugMesh.h"
//#include "ndTargaToOpenGl.h"
//#include "ndDemoEntityNotify.h"
//#include "ndAuthor.h"
//#include "ndDemoCameraManager.h"
//#include "ndDemoCameraManager.h"
//#include "ndAnimationSequence.h"
//#include "ndHighResolutionTimer.h"

#if 0
//#define DEFAULT_SCENE	0		// basic rigidbody
//#define DEFAULT_SCENE	1		// gpu basic rigidbody
//#define DEFAULT_SCENE	2		// friction ramp
//#define DEFAULT_SCENE	3		// basic compound shapes
#define DEFAULT_SCENE	4		// conservation of momentum 
//#define DEFAULT_SCENE	5		// basic Stacks
//#define DEFAULT_SCENE	6		// basic Trigger
//#define DEFAULT_SCENE	7		// particle fluid
//#define DEFAULT_SCENE	8		// static mesh collision 
//#define DEFAULT_SCENE	9		// static user mesh collision 
//#define DEFAULT_SCENE	10		// basic joints
//#define DEFAULT_SCENE	11		// basic vehicle
//#define DEFAULT_SCENE	12		// heavy vehicle
//#define DEFAULT_SCENE	13		// background vehicle prop
//#define DEFAULT_SCENE	14		// simple industrial robot
//#define DEFAULT_SCENE	15		// advanced industrial robot
//#define DEFAULT_SCENE	16		// basic player
//#define DEFAULT_SCENE	17		// rag doll
//#define DEFAULT_SCENE	18		// zero moment point
//#define DEFAULT_SCENE	19		// quadruped test 1
//#define DEFAULT_SCENE	20		// quadruped test 2
//#define DEFAULT_SCENE	21		// quadruped test 3
//#define DEFAULT_SCENE	22		// biped test 1
//#define DEFAULT_SCENE	23		// biped test 2
//#define DEFAULT_SCENE	24		// train biped test 2
//#define DEFAULT_SCENE	25		// simple voronoi fracture
//#define DEFAULT_SCENE	26		// basic voronoi fracture
//#define DEFAULT_SCENE	27		// linked voronoi fracture
//#define DEFAULT_SCENE	28		// skin peel voronoi fracture
						 
// demos forward declaration 
void ndRagdollTest(ndAuthor* const scene);
void ndBipedTest_1(ndAuthor* const scene);
void ndBipedTest_2(ndAuthor* const scene);
void ndBasicStacks(ndAuthor* const scene);
void ndBasicJoints(ndAuthor* const scene);
void ndBasicVehicle(ndAuthor* const scene);
void ndHeavyVehicle(ndAuthor* const scene);
void ndBasicTrigger(ndAuthor* const scene);
void ndBasicGpuTest0(ndAuthor* const scene);
void ndBasicRigidBody(ndAuthor* const scene);
void ndZeroMomentPoint(ndAuthor* const scene);
void ndQuadrupedTest_1(ndAuthor* const scene);
void ndQuadrupedTest_2(ndAuthor* const scene);
void ndQuadrupedTest_3(ndAuthor* const scene);
void ndBasicGpuRigidBody(ndAuthor* const scene);
void ndBasicFrictionRamp(ndAuthor* const scene);
void ndPlayerCapsuleDemo(ndAuthor* const scene);
void ndBipedTest_2Trainer(ndAuthor* const scene);
void ndBasicParticleFluid(ndAuthor* const scene);
void ndBasicAngularMomentum(ndAuthor* const scene);
void ndBagroundLowLodVehicle(ndAuthor* const scene);
void ndSimpleIndustrialRobot(ndAuthor* const scene);
void ndBasicCompoundShapeDemo(ndAuthor* const scene);
void ndAdvancedIndustrialRobot(ndAuthor* const scene);
void ndBasicExplodeConvexShape(ndAuthor* const scene);
//void ndBasicFracture_0(ndAuthor* const scene);
//void ndBasicFracture_2(ndAuthor* const scene);
//void ndBasicFracture_4(ndAuthor* const scene);
void ndStaticMeshCollisionDemo(ndAuthor* const scene);
void ndStaticUserMeshCollisionDemo(ndAuthor* const scene);

ndAuthor::SDKDemos ndAuthor::m_demosSelection[] = 
{
	{ "basic rigidbody", ndBasicRigidBody },
	{ "basic gpu rigidbody", ndBasicGpuRigidBody },
	{ "basic friction ramp", ndBasicFrictionRamp },
	{ "basic compound shapes", ndBasicCompoundShapeDemo },
	{ "basic conservation of momentum", ndBasicAngularMomentum },
	{ "basic stack", ndBasicStacks },
	{ "basic trigger", ndBasicTrigger },
	{ "basic particle fluid", ndBasicParticleFluid },
	{ "static mesh", ndStaticMeshCollisionDemo },
	{ "static user mesh", ndStaticUserMeshCollisionDemo },
	{ "basic joints", ndBasicJoints },
	{ "basic vehicle", ndBasicVehicle },
	{ "heavy vehicle", ndHeavyVehicle },
	{ "low lod vehicle", ndBagroundLowLodVehicle },
	{ "simple industrial robot", ndSimpleIndustrialRobot },
	{ "advanced industrial robot", ndAdvancedIndustrialRobot },
	{ "basic player", ndPlayerCapsuleDemo },
	{ "rag doll", ndRagdollTest },
	{ "zero moment point", ndZeroMomentPoint },
	{ "quadruped test one", ndQuadrupedTest_1 },
	{ "quadruped test two", ndQuadrupedTest_2 },
	{ "quadruped test three", ndQuadrupedTest_3},
	{ "biped test one", ndBipedTest_1 },
	{ "biped test two", ndBipedTest_2 },
	{ "train biped test two", ndBipedTest_2Trainer },
	{ "simple convex fracture", ndBasicExplodeConvexShape },
	//{ "basic convex fracture", ndBasicFracture_0 },
	//{ "linked convex fracture", ndBasicFracture_2 },
	//{ "simple skin peeling fracture", ndBasicFracture_4 },
};

ndAuthor::ButtonKey::ButtonKey (bool state)
	:m_state(state)
	,m_memory0(false)
	,m_memory1(false)
{
}

ndInt32 ndAuthor::ButtonKey::UpdateTrigger (bool triggerValue)
{
	m_memory0 = m_memory1;
	m_memory1 = triggerValue;
	return (!m_memory0 & m_memory1) ? 1 : 0;
}

ndInt32 ndAuthor::ButtonKey::UpdatePushButton (bool triggerValue)
{
	if (UpdateTrigger (triggerValue)) 
	{
		m_state = ! m_state;
	}
	return m_state ? 1 : 0;
}

void Test0__()
{
	ndFloat32 x[] = { 1.0f, -2.0f, 1.0f, 2.5f, 3.0f, -1.0f };
	//ndFloat32 x[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	ndFloat32 A[6][6];
	ndFloat32 B[6];

	ndCovarianceMatrix<ndFloat32>(6, &A[0][0], x, x);
	for (ndInt32 i = 0; i < 6; ++i)
	{
		A[i][i] *= 1.1f;
	}
	ndAssert(ndTestPSDmatrix(6, 6, &A[0][0]));

	ndMatrixTimeVector<ndFloat32>(6, &A[0][0], x, B);
	for (ndInt32 i = 0; i < 6; ++i)
	{
		x[i] = 0;
	}

	ndFloat32 precond[6 * 2];
	ndConjugateGradient<ndFloat32> cgd;
	cgd.Solve(6, 1.0e-5f, x, B, &A[0][0], precond);
}

void Test1__()
{
	//ndFloat32 A[2][2];
	//ndFloat32 x[2];
	//ndFloat32 b[2];
	//ndFloat32 l[2];
	//ndFloat32 h[2];
	//
	//A[0][0] = 2.0f;
	//A[0][1] = 1.0f;
	//A[1][0] = 1.0f;
	//A[1][1] = 2.0f;
	//b[0] = 1.0f;
	//b[1] = 1.0f;
	//x[0] = 1;
	//x[1] = 2;
	//
	//l[0] = 0.0f;
	//l[1] = 0.0f;
	//h[0] = 0.25f;
	//h[1] = 1.0f;
	//
	//ndMatrixTimeVector(2, &A[0][0], x, b);
	//dSolveDantzigLCP(2, &A[0][0], x, b, l, h);
	//
	//ndInt32 xxx = 0;
	//const ndInt32 xxxxxx = 450;
	//dDownHeap<ndInt32, unsigned> xxxxx (xxxxxx + 2);
	//for (ndInt32 i = 0; i < xxxxxx; ++i)
	//{
	//	xxxxx.Push (xxx, i);
	//}
	//
	//for (ndInt32 i = 0; i < 10000; ++i)
	//{
	//	ndInt32 index = dRandInt() % xxxxxx;
	//	ndInt32 key = xxxxx.Value(index);
	//	xxxxx.Remove (index);
	//	xxxxx.Push (xxx, key);
	//}
}

class ndAuthor::ndAuthor::ndDebuMesh
{
	public:
	ndDebuMesh()
		:m_flatShaded()
		,m_wireFrameOpenEdge()
		,m_wireFrameShareEdge()
	{
	}

	ndSharedPtr<ndFlatShadedDebugMesh> m_flatShaded;
	ndSharedPtr<ndWireFrameDebugMesh> m_wireFrameOpenEdge;
	ndSharedPtr<ndWireFrameDebugMesh> m_wireFrameShareEdge;
};

class ndAuthor::ndAuthor::ndDebugMeshCache : public ndTree<ndDebuMesh, const ndShape*>
{
};

// ImGui - standalone example application for Glfw + OpenGL 2, using fixed pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
ndAuthor::ndAuthor ()
	:m_mainFrame(nullptr)
	,m_defaultFont(0)
	,m_sky(nullptr)
	,m_world(nullptr)
	,m_cameraManager(nullptr)
	,m_updateCameraContext(nullptr)
	,m_renderDemoGUI()
	,m_updateCamera(nullptr)
	,m_microsecunds(0)
	,m_transparentHeap()
	,m_animationCache()
	,m_currentScene(DEFAULT_SCENE)
	,m_lastCurrentScene(DEFAULT_SCENE)
	,m_framesCount(0)
	,m_physicsFramesCount(0)
	,m_currentPlugin(0)
	,m_solverPasses(4)
	,m_solverSubSteps(2)
	,m_workerThreads(1)
	,m_debugDisplayMode(0)
	,m_collisionDisplayMode(0)
	,m_selectedModel(nullptr)
	,m_fps(0.0f)
	,m_timestepAcc(0.0f)
	,m_currentListenerTimestep(0.0f)
	,m_addDeleteLock()
	,m_showUI(true)
	,m_showAABB(false)
	,m_showStats(true)
	,m_autoSleepMode(true)
	,m_showScene(false)
	,m_showConcaveEdge(false)
	,m_hideVisualMeshes(false)
	,m_showNormalForces(false)
	,m_showCenterOfMass(false)
	,m_showBodyFrame(false)
	,m_showMeshSkeleton(false)
	,m_updateMenuOptions(true)
	,m_showContactPoints(false)
	,m_showJointDebugInfo(false)
	,m_showModelsDebugInfo(false)
	,m_showCollidingFaces(false)
	,m_suspendPhysicsUpdate(false)
	,m_synchronousPhysicsUpdate(false)
	,m_synchronousParticlesUpdate(false)
	,m_showRaycastHit(false)
	,m_profilerMode(false)
	,m_solverMode(ndWorld::ndSimdSoaSolver)
	,m_debugShapeCache(new ndDebugMeshCache())
	,m_replayLogFile(nullptr)
{
	// Setup window
	glfwSetErrorCallback(ErrorCallback);
	glfwInit();

	// Decide GL+GLSL versions
	#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
	#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	#endif

	#if defined (_DEBUG)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif

	//m_hasJoytick = glfwJoystickPresent(0) ? true : false;

	// Create window with graphics context
	char version[256];
	sprintf(version, "Newton Dynamics %d.%.2i sandbox demos", D_NEWTON_ENGINE_MAJOR_VERSION, D_NEWTON_ENGINE_MINOR_VERSION);
	//GLFWwindow* window = glfwCreateWindow(1280, 720, version, NULL, NULL);
	m_mainFrame = glfwCreateWindow(1280, 720, version, NULL, NULL);
	glfwMakeContextCurrent(m_mainFrame);
	glfwSwapInterval(0); // Enable vsync

	 // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

	ndInt32 monitorsCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);
	if (monitorsCount > 1)
	{
		ndInt32 window_x;
		ndInt32 window_y;
		ndInt32 monitor_x;
		ndInt32 monitor_y;

		glfwGetMonitorPos(monitors[1], &monitor_x, &monitor_y);
		glfwGetWindowPos(m_mainFrame, &window_x, &window_y);
		glfwSetWindowPos(m_mainFrame, monitor_x + window_x, monitor_y + 64);
	}

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();
	ImGuiStyle* const style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.5f);

	// Setup Platform/Renderer back ends
	ImGui_ImplGlfw_InitForOpenGL(m_mainFrame, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// attach myself to the main frame
	glfwSetWindowUserPointer(m_mainFrame, this);

	#if defined (_DEBUG)
	glDebugMessageCallback((GLDEBUGPROC)OpenMessageCallback, m_mainFrame);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	#endif

	// Load Fonts
	LoadFont();

	glfwSetKeyCallback(m_mainFrame, KeyCallback);
	glfwSetCharCallback(m_mainFrame, CharCallback);
	glfwSetScrollCallback(m_mainFrame, MouseScrollCallback);
	glfwSetCursorPosCallback(m_mainFrame, CursorposCallback);
	glfwSetMouseButtonCallback(m_mainFrame, MouseButtonCallback);

	// Setup ImGui binding
	io.UserData = this;

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	#ifdef _MSC_VER 
	io.ImeWindowHandle = glfwGetWin32Window(m_mainFrame);
	#endif

	m_mousePressed[0] = false;
	m_mousePressed[1] = false;
	m_mousePressed[2] = false;
	
	// initialized the physics world for the new scene
	//m_showUI = false;
	//m_showAABB = true;
	//m_hideVisualMeshes = true;
	//m_showScene = true;
	//m_showConcaveEdge = true;
	//m_autoSleepMode = false;
	//m_solverMode = ndWorld::ndCudaSolver;
	//m_solverMode = ndWorld::ndSimdSoaSolver;
	//m_solverMode = ndWorld::ndSyclSolverCpu;
	m_solverMode = ndWorld::ndStandardSolver;
	//m_solverMode = ndWorld::ndSimdAvx2Solver;
	//m_solverPasses = 4;
	m_workerThreads = 1;
	//m_solverSubSteps = 2;
	//m_showRaycastHit = true;
	//m_showCenterOfMass = false;
	//m_showNormalForces = true;
	//m_showContactPoints = true;
	//m_showJointDebugInfo = true;
	//m_showModelsDebugInfo = true;
	//m_collisionDisplayMode = 1;
	//m_collisionDisplayMode = 2;	
	//m_collisionDisplayMode = 3;		// solid wire frame
	m_synchronousPhysicsUpdate = true;
	m_synchronousParticlesUpdate = true;

	Cleanup();
	ResetTimer();

	m_shaderCache.CreateAllEffects();

	#ifdef ENABLE_REPLAY
		#ifdef REPLAY_RECORD
			m_replayLogFile = fopen("replayLog.bin", "wb");
		#else 
			m_replayLogFile = fopen("replayLog.bin", "rb");
		#endif
	#endif

	//Test0__();
	//Test1__();
	ndTestDeedBrian();

	//ndSharedPtr<ndAuthor> xxx(this);
	//ndAuthor* xxx1 = *xxx;
	//ndAuthor* xxx2 = *xxx;
}

ndAuthor::~ndAuthor ()
{
	if (m_replayLogFile)
	{
		fclose(m_replayLogFile);
	}

	Cleanup ();

	if (m_debugShapeCache)
	{
		delete m_debugShapeCache;
	}

	if (m_cameraManager)
	{
		delete m_cameraManager;
	}

	// destroy the empty world
	if (m_world) 
	{
		delete m_world;
	}

	// Cleanup
	GLuint font_texture = GLuint(m_defaultFont);
	glDeleteTextures(1, &font_texture);
	ImGui::GetIO().Fonts->TexID = 0;

	m_shaderCache.Cleanup();
	TextureCacheCleanUp();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_mainFrame);
	glfwTerminate();
}

#ifdef _DEBUG
void ndAuthor::OpenMessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (userParam)
	{
		switch(id)
		{
			case 131185:  // nvidia driver report will use VIDEO memory as the source for buffer object operations
				return;
		}
		ndTrace(("GL CALLBACK: %s source = 0x%x, type = 0x%x, id = %d, severity = 0x%x, message = %s, length = %d \n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), source, type, id, severity, message, length));
		ndAssert(0);
	}
}
#endif

ndDemoCamera* ndAuthor::GetCamera() const
{
	return m_cameraManager->GetCamera();
}

bool ndAuthor::GetKeyState(ndInt32 key) const
{
	const ImGuiIO& io = ImGui::GetIO();
	bool state = io.KeysDown[key];
#ifdef ENABLE_REPLAY
	#ifdef REPLAY_RECORD
		ndInt32 value = state;
		fwrite(&value, sizeof(ndInt32), 1, m_replayLogFile);
		fflush(m_replayLogFile);
	#else
		ndInt32 value;
		fread(&value, sizeof(ndInt32), 1, m_replayLogFile);
		state = value ? 1 : 0;
	#endif	
#endif
	return state;
}

ndAnimationSequence* ndAuthor::GetAnimationSequence(const char* const fileName)
{
	ndTree<ndAnimationSequence*, ndString>::ndNode* node = m_animationCache.Find(fileName);
	if (!node)
	{
		ndAnimationSequence* const sequence = LoadFbxAnimation(fileName);
		if (sequence)
		{
			node = m_animationCache.Insert(sequence, fileName);
		}
	}
	return node ? node->GetInfo() : nullptr;
}

bool ndAuthor::IsShiftKeyDown () const
{
	const ImGuiIO& io = ImGui::GetIO();
	bool state = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	return state;
}

bool ndAuthor::IsControlKeyDown () const
{
	const ImGuiIO& io = ImGui::GetIO();
	bool state = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	return state;
}

bool ndAuthor::GetCaptured() const
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse;
}

bool ndAuthor::GetMouseKeyState (ndInt32 button) const
{
	ImGuiIO& io = ImGui::GetIO();
	return io.MouseDown[button];
}

void ndAuthor::Set2DDisplayRenderFunction(ndSharedPtr<ndUIEntity>& demoGui)
{
	m_renderDemoGUI = demoGui;
}

void* ndAuthor::GetUpdateCameraContext() const
{
	return m_updateCameraContext;
}

void ndAuthor::SetUpdateCameraFunction(UpdateCameraCallback callback, void* const context)
{
	m_updateCamera = callback;
	m_updateCameraContext = context;
}

bool ndAuthor::JoystickDetected() const
{
	return glfwJoystickPresent(0) ? true : false;
}

void ndAuthor::GetJoystickAxis (ndFixSizeArray<ndFloat32, 8>& axisValues)
{
	ndAssert(JoystickDetected());
	ndInt32 axisCount = 0;
	axisValues.SetCount(0);
	const float* const axis = glfwGetJoystickAxes(0, &axisCount);
	axisCount = ndMin (axisCount, axisValues.GetCapacity());
	for (ndInt32 i = 0; i < axisCount; ++i) 
	{
		axisValues.PushBack(axis[i]);
	}

#ifdef ENABLE_REPLAY
	#ifdef REPLAY_RECORD
		fwrite(&axisCount, sizeof(axisCount), 1, m_replayLogFile);
		fwrite(&axisValues[0], sizeof(ndFloat32) * axisValues.GetCapacity(), 1, m_replayLogFile);
		fflush(m_replayLogFile);
	#else 
		fread(&axisCount, sizeof(axisCount), 1, m_replayLogFile);
		fread(&axisValues[0], sizeof(ndFloat32) * axisValues.GetCapacity(), 1, m_replayLogFile);
	#endif
#endif
}

void ndAuthor::GetJoystickButtons(ndFixSizeArray<char, 32>& axisbuttons)
{
	ndAssert(JoystickDetected());
	ndInt32 buttonsCount = 0;
	axisbuttons.SetCount(0);
	const unsigned char* const buttons = glfwGetJoystickButtons(0, &buttonsCount);
	buttonsCount = ndMin (buttonsCount, axisbuttons.GetCapacity());

	for (ndInt32 i = 0; i < buttonsCount; ++i) 
	{
		axisbuttons.PushBack(char(buttons[i]));
		//if (buttons[i]) ndTrace(("%d %d\n", i, buttons[i]));
	}

#ifdef ENABLE_REPLAY
	#ifdef REPLAY_RECORD
		fwrite(&buttonsCount, sizeof(buttonsCount), 1, m_replayLogFile);
		fwrite(&axisbuttons[0], sizeof(axisbuttons.GetCapacity()), 1, m_replayLogFile);
		fflush(m_replayLogFile);
	#else 
		fread(&buttonsCount, sizeof(buttonsCount), 1, m_replayLogFile);
		fread(&axisbuttons[0], sizeof(axisbuttons.GetCapacity()), 1, m_replayLogFile);
	#endif
#endif
}

void ndAuthor::ResetTimer()
{
	dResetTimer();
	m_microsecunds = ndGetTimeInMicroseconds ();
}

void ndAuthor::AddEntity(ndDemoEntity* const ent)
{
	ndScopeSpinLock lock(m_addDeleteLock);
	ndAssert(!ent->m_rootNode);
	ent->m_rootNode = Append(ent);
}

void ndAuthor::RemoveEntity (ndDemoEntity* const ent)
{
	ndScopeSpinLock lock(m_addDeleteLock);
	ndAssert(ent->m_rootNode);
	Remove(ent->m_rootNode);
}

void ndAuthor::Cleanup ()
{
	// is we are run asynchronous we need make sure no update in on flight.
	if (m_world) 
	{
		m_world->Sync();
	}
	
	ndTree<ndAnimationSequence*, ndString>::Iterator iter(m_animationCache);
	for (iter.Begin(); iter; iter++)
	{
		delete *iter;
	}
	m_animationCache.RemoveAll();
	
	while (m_debugShapeCache->GetRoot())
	{
		m_debugShapeCache->Remove(m_debugShapeCache->GetRoot());
	}
	
	if (m_cameraManager) 
	{
		delete m_cameraManager;
		m_cameraManager = nullptr;
	}
	
	m_sky = nullptr;
	m_updateCamera = nullptr;
	
	// destroy the Newton world
	if (m_world) 
	{
		const ndBodyListView& bodyList = m_world->GetBodyList();
		for (ndBodyListView::ndNode* bodyNode = bodyList.GetFirst(); bodyNode; bodyNode = bodyNode->GetNext())
		{
			ndBodyKinematic* const body = bodyNode->GetInfo()->GetAsBodyKinematic();
			ndDemoEntityNotify* const callback = (ndDemoEntityNotify*)body->GetNotifyCallback();
			if (callback)
			{
				callback->m_entity = nullptr;
			}
		}
		
		// get serialization call back before destroying the world
		m_world->CleanUp();
		delete m_world;
	}
	
	// destroy all remaining visual objects
	while (GetFirst())
	{
		ndDemoEntity* const ent = GetFirst()->GetInfo();
		RemoveEntity(ent);
		delete ent;
	}
	
	// create the newton world
	m_world = new ndPhysicsWorld(this);
	
	// add the camera manager
	m_cameraManager = new ndDemoCameraManager(this);
	
	ApplyMenuOptions();
	
	// we start without 2d render
	m_renderDemoGUI = ndSharedPtr<ndUIEntity>();
}

void ndAuthor::LoadFont()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();

    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //io.Fonts->AddFontDefault();

	float pixedSize = 18;
	char pathName[2048];
	const char* const name = "Cousine-Regular.ttf";
	//char* const name = "calibri.ttf";
	//char* const name = "courbd.ttf";

	dGetWorkingFileName (name, pathName);
    io.Fonts->AddFontFromFileTTF(pathName, pixedSize);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) 
	// because it is more likely to be compatible with user's existing shaders. 
	// If your ImTextureId represent a higher-level concept than just a GL texture id, 
	// consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
	unsigned char* pixels;
	ndInt32 width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   

	// Upload texture to graphics system
	GLint last_texture;
	GLuint font_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &font_texture);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	m_defaultFont = GLint(font_texture);
	io.Fonts->TexID = (void *)(intptr_t)m_defaultFont;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, GLuint(last_texture));
}

void ndAuthor::ApplyMenuOptions()
{
	m_world->Sync();
	m_world->SetSubSteps(m_solverSubSteps);
	m_world->SetSolverIterations(m_solverPasses);
	m_world->SetThreadCount(m_workerThreads);
	
	bool state = m_autoSleepMode ? true : false;
	const ndBodyListView& bodyList = m_world->GetBodyList();
	for (ndBodyListView::ndNode* node = bodyList.GetFirst(); node; node = node->GetNext())
	{
		ndBodyKinematic* const body = node->GetInfo()->GetAsBodyKinematic();
		body->SetAutoSleep(state);
	}
	
	SetParticleUpdateMode();
	m_world->SelectSolver(m_solverMode);
	m_solverMode = m_world->GetSelectedSolver();
}

void ndAuthor::ShowMainMenuBar()
{
	ndMenuSelection menuSelection = m_none;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File")) 
		{
			m_suspendPhysicsUpdate = true;

			if (ImGui::MenuItem("Preferences", "")) 
			{
				ndAssert (0);
			}
			ImGui::Separator();

			if (ImGui::MenuItem("New", "")) 
			{
				menuSelection = m_new;
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Open", "")) 
			{
				menuSelection = m_load;
			}
			if (ImGui::MenuItem("Save", "")) 
			{
				menuSelection = m_save;
			}
			if (ImGui::MenuItem("Save model", ""))
			{
				menuSelection = m_saveModel;
			}

			ImGui::Separator();
			if (ImGui::MenuItem("import ply file", "")) 
			{
				//mainMenu = 4;
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "")) 
			{
				glfwSetWindowShouldClose (m_mainFrame, 1);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Demos")) 
		{
			m_suspendPhysicsUpdate = true;
			ndInt32 demosCount = ndInt32 (sizeof (m_demosSelection) / sizeof m_demosSelection[0]);
			for (ndInt32 i = 0; i < demosCount; ++i) 
			{
				if (ImGui::MenuItem(m_demosSelection[i].m_name, "")) 
				{
					m_currentScene = i;
				}
			}

			ImGui::EndMenu();
		}

		bool optionsOn = ImGui::BeginMenu("Options");
		if (optionsOn) 
		{
			m_updateMenuOptions = true;
			m_suspendPhysicsUpdate = true;

			ImGui::Checkbox("auto sleep mode", &m_autoSleepMode);
			ImGui::Checkbox("show UI", &m_showUI);
			ImGui::Checkbox("show stats", &m_showStats);
			ImGui::Checkbox("synchronous physics update", &m_synchronousPhysicsUpdate);
			ImGui::Checkbox("synchronous particle update", &m_synchronousParticlesUpdate);
			ImGui::Separator();

			ImGui::Text("solvers");
			ndInt32 solverMode(m_solverMode);
			ImGui::RadioButton("default", &solverMode, ndWorld::ndStandardSolver);
			ImGui::RadioButton("sse", &solverMode, ndWorld::ndSimdSoaSolver);
			ImGui::RadioButton("avx2", &solverMode, ndWorld::ndSimdAvx2Solver);
			ImGui::RadioButton("cuda", &solverMode, ndWorld::ndCudaSolver);
			ImGui::RadioButton("syclCpu", &solverMode, ndWorld::ndSyclSolverCpu);
			ImGui::RadioButton("syclGpu", &solverMode, ndWorld::ndSyclSolverGpu);

			m_solverMode = ndWorld::ndSolverModes(solverMode);
			ImGui::Separator();

			ImGui::Text("solver sub steps");
			ImGui::SliderInt("##solv", &m_solverSubSteps, 2, 8);
			ImGui::Text("iterative solver passes");
			ImGui::SliderInt("##intera", &m_solverPasses, 4, 32);
			ImGui::Text("worker threads");
			ImGui::SliderInt("##worker", &m_workerThreads, 1, ndThreadPool::GetMaxThreads());
			ImGui::Separator();

			ImGui::RadioButton("hide collision Mesh", &m_collisionDisplayMode, 0);
			ImGui::RadioButton("show solid collision", &m_collisionDisplayMode, 1);
			ImGui::RadioButton("show wire frame collision", &m_collisionDisplayMode, 2);
			ImGui::RadioButton("show hidden wire frame collision", &m_collisionDisplayMode, 3);
			ImGui::Separator();

			ImGui::Checkbox("show aabb", &m_showAABB);
			ImGui::Checkbox("show broad phase", &m_showScene);
			ImGui::Checkbox("show concave edge", &m_showConcaveEdge);
			ImGui::Checkbox("hide visual meshes", &m_hideVisualMeshes);
			ImGui::Checkbox("show mesh skeleton", &m_showMeshSkeleton);
			ImGui::Checkbox("show contact points", &m_showContactPoints);
			ImGui::Checkbox("show ray cast hit point", &m_showRaycastHit);
			ImGui::Checkbox("show normal forces", &m_showNormalForces);
			ImGui::Checkbox("show center of mass", &m_showCenterOfMass);
			ImGui::Checkbox("show body frame", &m_showBodyFrame);
			ImGui::Checkbox("show joints debug info", &m_showJointDebugInfo);
			ImGui::Checkbox("show models debug info", &m_showModelsDebugInfo);
			ImGui::Checkbox("show colliding faces", &m_showCollidingFaces);

			ImGui::EndMenu();

			SetParticleUpdateMode();
		}

		if (ImGui::BeginMenu("Help")) 
		{
			m_suspendPhysicsUpdate = true;
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		if (!optionsOn && m_updateMenuOptions) 
		{
			m_updateMenuOptions = false;
			ApplyMenuOptions();
		}
	}

	switch (menuSelection)
	{
		case m_new:
		{
			// menu new 
			Cleanup();
			ApplyMenuOptions();
			ResetTimer();
			m_currentScene = -1;
			break;
		}

		case m_load:
		{
			m_currentScene = -1;
			char fileName[1024];
			if (dGetLoadNdFileName(fileName, 1024))
			{
				ndAssert(0);
				//m_world->LoadScene(fileName);
			}
			break;
		}

		case m_save:
		{
			m_currentScene = -1;
			char fileName[1024];
			if (dGetSaveNdFileName(fileName, 1024))
			{
				ndAssert(0);
				//m_world->SaveScene(fileName);
			}
			break;
		}

		case m_saveModel:
		{
			m_currentScene = -1;
			char fileName[1024];
			if (dGetSaveNdFileName(fileName, 1024))
			{
				ndAssert(0);
				//m_world->SaveSceneModel(fileName);
			}
			break;
		}

		case m_none:
		default:
		{
			// load a demo 
			if (m_currentScene != -1) 
			{
				m_selectedModel = nullptr;
				LoadDemo(m_currentScene);
				m_lastCurrentScene = m_currentScene;
				m_currentScene = -1;
			}
		}
	}
}

void ndAuthor::LoadDemo(ndInt32 menu)
{
	char newTitle[256];
	Cleanup();

	// make the sky box 
	CreateSkyBox();
	m_demosSelection[menu].m_launchDemoCallback(this);

	sprintf(newTitle, "Newton Dynamics %d.%.2i demo: %s", D_NEWTON_ENGINE_MAJOR_VERSION, D_NEWTON_ENGINE_MINOR_VERSION, m_demosSelection[menu].m_name);
	glfwSetWindowTitle(m_mainFrame, newTitle);
	ApplyMenuOptions();
	ResetTimer();
}

void ndAuthor::ErrorCallback(ndInt32 error, const char* description)
{
	ndTrace (("Error %d: %s\n", error, description));
	fprintf(stderr, "Error %d: %s\n", error, description);
	ndAssert (0);
}

void ndAuthor::MouseButtonCallback(GLFWwindow*, ndInt32 button, ndInt32 action, ndInt32)
{
	if (button >= 0 && button < 3) 
	{
		ImGuiIO& io = ImGui::GetIO();
		if (action == GLFW_PRESS) 
		{
			io.MouseDown[button] = true;    
		} 
		else if (action == GLFW_RELEASE) 
		{
			io.MouseDown[button] = false;    
		}
	}
}

void ndAuthor::MouseScrollCallback(GLFWwindow* const, double, double y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += float (y);
}

void ndAuthor::CursorposCallback  (GLFWwindow* , double x, double y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
}

bool ndAuthor::GetMouseSpeed(ndFloat32& speedX, ndFloat32& speedY) const
{
	ImVec2 speed(ImGui::GetMouseDragDelta(0, 0.0f));
	speedX = speed.x;
	speedY = speed.y;
	return true;
}

bool ndAuthor::GetMousePosition (ndFloat32& posX, ndFloat32& posY) const
{
	ImVec2 posit(ImGui::GetMousePos());
	posX = posit.x;
	posY = posit.y;
	return true;
}

void ndAuthor::CharCallback(GLFWwindow*, ndUnsigned32 ch)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter((unsigned short)ch);
}


void ndAuthor::KeyCallback(GLFWwindow* const window, ndInt32 key, ndInt32, ndInt32 action, ndInt32 mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;

	(void)mods; // Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
	static ndInt32 prevKey;
	ndAuthor* const manager = (ndAuthor*)glfwGetWindowUserPointer(window);
	if ((key == GLFW_KEY_F10) && (key != prevKey)) 
	{
		manager->m_profilerMode = true;
	}

	if (key == GLFW_KEY_ESCAPE) 
	{
		glfwSetWindowShouldClose (window, 1);
	}

	if (key == GLFW_KEY_F1) 
	{
		ndMatrix cameraMatrix(manager->GetCamera()->GetCurrentMatrix());
		manager->LoadDemo(manager->m_lastCurrentScene);
		manager->SetCameraMatrix(cameraMatrix, cameraMatrix.m_posit);
	}

	prevKey = io.KeysDown[key] ? key : 0;
}

void ndAuthor::ToggleProfiler()
{
	#ifdef D_PROFILER
		ndAssert(m_world);
		ndTrace(("profiler Enable\n"));
		m_world->Sync();
		dProfilerEnableProling();
	#endif
}

void ndAuthor::BeginFrame()
{
	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	glfwPollEvents();

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	ndInt32 w, h;
	ndInt32 display_w, display_h;
	glfwGetWindowSize(m_mainFrame, &w, &h);
	glfwGetFramebufferSize(m_mainFrame, &display_w, &display_h);
	io.DisplaySize = ImVec2((ndReal)w, (ndReal)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((ndReal)display_w / (ndReal)w) : 0, h > 0 ? ((ndReal)display_h / (ndReal)h) : 0);

	//int display_w, display_h;
	//glfwGetFramebufferSize(m_mainFrame, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

ndInt32 ndAuthor::ParticleCount() const
{
	ndInt32 count = 0;
	const ndBodyList& particles = m_world->GetParticleList();
	for (ndBodyList::ndNode* node = particles.GetFirst(); node; node = node->GetNext())
	{
		ndBodyParticleSet* const set = node->GetInfo()->GetAsBodyParticleSet();
		count += set->GetPositions().GetCount();
	}
	return count;
}

void ndAuthor::SetParticleUpdateMode() const
{
	const ndBodyList& particles = m_world->GetParticleList();
	for (ndBodyList::ndNode* node = particles.GetFirst(); node; node = node->GetNext())
	{
		ndBodyParticleSet* const set = node->GetInfo()->GetAsBodyParticleSet();
		set->SetAsynUpdate(!m_synchronousParticlesUpdate);
	}
}

void ndAuthor::RenderStats()
{
	if (m_showStats) 
	{
		char text[1024];
		
		if (ImGui::Begin("statistics", &m_showStats)) 
		{
			sprintf (text, "fps:            %6.3f", m_fps);
			ImGui::Text(text, "");

			sprintf(text, "physics time:  %6.3f ms", m_world->GetAverageUpdateTime() * 1.0e3f);
			ImGui::Text(text, "");

			sprintf(text, "update mode:    %s", m_synchronousPhysicsUpdate ? "synchronous" : "asynchronous");
			ImGui::Text(text, "");

			sprintf(text, "particle mode:  %s", m_synchronousParticlesUpdate ? "synchronous" : "asynchronous");
			ImGui::Text(text, "");

			sprintf(text, "bodies:         %d", m_world->GetBodyList().GetCount());
			ImGui::Text(text, "");

			sprintf(text, "joints:         %d", m_world->GetJointList().GetCount());
			ImGui::Text(text, "");

			sprintf(text, "contact joints: %d", m_world->GetContactList().GetCount());
			ImGui::Text(text, "");

			sprintf(text, "particles:      %d", ParticleCount());
			ImGui::Text(text, "");

			sprintf(text, "memory used:   %6.3f mbytes", ndFloat32(ndFloat64(ndMemory::GetMemoryUsed()) / (1024 * 1024)));
			ImGui::Text(text, "");

			sprintf(text, "threads:        %d", m_world->GetThreadCount());
			ImGui::Text(text, "");

			sprintf(text, "iterations:     %d", m_world->GetSolverIterations());
			ImGui::Text(text, "");

			sprintf(text, "Substeps:       %d", m_world->GetSubSteps());
			ImGui::Text(text, "");

			sprintf(text, "solver:         %s", m_world->GetSolverString());
			ImGui::Text(text, "");

			m_suspendPhysicsUpdate = m_suspendPhysicsUpdate || (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0));
			ImGui::End();
		}
	}

	if (m_showUI && *m_renderDemoGUI)
	{
		if (ImGui::Begin("User Interface", &m_showUI))
		{
			m_renderDemoGUI->RenderHelp();
			ImGui::End();
		}
	}

	ShowMainMenuBar();
}

void ndAuthor::CalculateFPS(ndFloat32 timestep)
{
	m_framesCount ++;
	m_timestepAcc += timestep;

	// this probably happing on loading of and a pause, just rest counters
	if ((m_timestepAcc <= 0.0f) || (m_timestepAcc > 4.0f))
	{
		m_timestepAcc = 0;
		m_framesCount = 0;
	}

	//update fps every quarter of a second
	const ndFloat32 movingAverage = 0.5f;
	if (m_timestepAcc >= movingAverage)
	{
		m_fps = ndFloat32 (m_framesCount) / m_timestepAcc;
		m_timestepAcc -= movingAverage;
		m_framesCount = 0;
	}
}

void ndAuthor::CreateSkyBox()
{
	if (!m_sky)
	{
		m_sky = new ndSkyBox(m_shaderCache.m_skyBox);
		
		ndScopeSpinLock lock(m_addDeleteLock);
		ndAssert(!m_sky->m_rootNode);
		m_sky->m_rootNode = Addtop(m_sky);
	}
}

void ndAuthor::PushTransparentMesh (const ndDemoMeshInterface* const mesh, const ndMatrix& modelMatrix)
{
	ndVector dist (m_cameraManager->GetCamera()->GetViewMatrix().TransformVector(modelMatrix.m_posit));
	TransparentMesh entry (modelMatrix, (ndDemoMesh*) mesh);
	m_transparentHeap.Push (entry, dist.m_z);
}


//void ndAuthor::ImportPLYfile (const char* const fileName)
void ndAuthor::ImportPLYfile(const char* const)
{
	ndAssert(0);
	//m_collisionDisplayMode = 2;
	//CreatePLYMesh (this, fileName, true);
}

ndInt32 ndAuthor::Print (const ndVector&, const char *fmt, ... ) const
{
	va_list argptr;
	char string[1024];

	va_start (argptr, fmt);
	vsprintf (string, fmt, argptr);
	va_end( argptr );
	ImGui::Text(string, "");
	return 0;
}

void ndAuthor::SetCameraMatrix (const ndQuaternion& rotation, const ndVector& position)
{
	m_cameraManager->SetCameraMatrix(rotation, position);
}

void ndAuthor::UpdatePhysics(ndFloat32 timestep)
{
	// update the physics
	if (m_world && !m_suspendPhysicsUpdate) 
	{
		m_world->AdvanceTime(timestep);
	}
}

ndFloat32 ndAuthor::CalculateInteplationParam () const
{
	ndUnsigned64 timeStep = ndGetTimeInMicroseconds () - m_microsecunds;		
	ndFloat32 param = (ndFloat32 (timeStep) * MAX_PHYSICS_FPS) / 1.0e6f;
	ndAssert (param >= 0.0f);
	if (param > 1.0f) 
	{
		param = 1.0f;
	}
	return param;
}


void ndAuthor::RenderScene(ImDrawData* const draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();

	ndInt32 fb_width = (ndInt32)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	ndInt32 fb_height = (ndInt32)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
	{
		return;
	}

	ndAuthor* const window = (ndAuthor*)io.UserData;

	ImVec4 clearColor = ImColor(114, 144, 154);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);

	window->RenderScene();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (*window->m_renderDemoGUI) 
	{
		window->m_renderDemoGUI->RenderUI();
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Render command lists
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);
	//#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	for (ndInt32 n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
		const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
		glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)((char*)vtx_buffer + OFFSETOF(ImDrawVert, pos)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)((char*)vtx_buffer + OFFSETOF(ImDrawVert, uv)));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)((char*)vtx_buffer + OFFSETOF(ImDrawVert, col)));

		for (ndInt32 cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((ndInt32)pcmd->ClipRect.x, (ndInt32)((ndFloat32)fb_height - pcmd->ClipRect.w), (ndInt32)(pcmd->ClipRect.z - pcmd->ClipRect.x), (ndInt32)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
			}
			idx_buffer += pcmd->ElemCount;
		}
	}
	#undef OFFSETOF

	// Restore modified state
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void ndAuthor::DrawDebugShapes()
{
	const ndVector awakeColor(1.0f, 1.0f, 1.0f, 1.0f);
	const ndVector sleepColor(0.42f, 0.73f, 0.98f, 1.0f);
	
	const ndBodyListView& bodyList = m_world->GetBodyList();
	
	if (m_collisionDisplayMode == 3)
	{
		// do a z buffer pre pass for hidden line 
		glColorMask(0, 0, 0, 0);
		for (ndBodyListView::ndNode* bodyNode = bodyList.GetFirst(); bodyNode; bodyNode = bodyNode->GetNext())
		{
			ndBodyKinematic* const body = bodyNode->GetInfo()->GetAsBodyKinematic();
			const ndShapeInstance& shapeInstance = body->GetCollisionShape();
			ndShape* const key = (ndShape*)shapeInstance.GetShape();
			if (key->GetAsShapeStaticProceduralMesh())
			{
				ndDebugNotify drawShapes(this, body);
				key->DebugShape(ndGetIdentityMatrix(), drawShapes);
			}
			else if (!body->GetAsBodyTriggerVolume())
			{
				ndDebugMeshCache::ndNode* const shapeNode = m_debugShapeCache->Find(key);
				if (shapeNode)
				{
					ndMatrix matrix(shapeInstance.GetScaledTransform(body->GetMatrix()));
					shapeNode->GetInfo().m_flatShaded->Render(this, matrix);
				}
			}
		}
		glColorMask(1, 1, 1, 1);
	}
	
	for (ndBodyListView::ndNode* bodyNode = bodyList.GetFirst(); bodyNode; bodyNode = bodyNode->GetNext())
	{
		ndBodyKinematic* const body = bodyNode->GetInfo()->GetAsBodyKinematic();
		const ndShapeInstance& shapeInstance = body->GetCollisionShape();
		ndShape* const key = (ndShape*)shapeInstance.GetShape();
		if (!(key->GetAsShapeNull() || key->GetAsShapeStaticProceduralMesh()))
		{
			ndDebugMeshCache::ndNode* shapeNode = m_debugShapeCache->Find(key);
			if (!shapeNode)
			{
				ndShapeInstance shape(body->GetCollisionShape());
				shape.SetScale(ndVector(1.0f));
				shape.SetLocalMatrix(ndGetIdentityMatrix());
	
				ndDebuMesh debugMesh;
				debugMesh.m_flatShaded = new ndFlatShadedDebugMesh(m_shaderCache, &shape);
				debugMesh.m_wireFrameShareEdge = new ndWireFrameDebugMesh(m_shaderCache, &shape);
				if (shape.GetShape()->GetAsShapeStaticBVH())
				{
					debugMesh.m_wireFrameOpenEdge = new ndWireFrameDebugMesh(m_shaderCache, &shape, ndShapeDebugNotify::ndEdgeType::m_open);
					debugMesh.m_wireFrameOpenEdge->SetColor(ndVector(1.0f, 0.0f, 1.0f, 1.0f));
				}
				shapeNode = m_debugShapeCache->Insert(debugMesh, key);
			}
	
			ndMatrix matrix(shapeInstance.GetScaledTransform(body->GetMatrix()));
			ndInt32 sleepState = body->GetSleepState();
			ndVector color((sleepState == 1) ? sleepColor : awakeColor);
	
			if (m_collisionDisplayMode >= 2)
			{
				ndWireFrameDebugMesh* const sharedEdgeMesh = *shapeNode->GetInfo().m_wireFrameShareEdge;
				sharedEdgeMesh->SetColor(color);
				sharedEdgeMesh->Render(this, matrix);
	
				if (*shapeNode->GetInfo().m_wireFrameOpenEdge)
				{
					ndWireFrameDebugMesh* const openEdgeMesh = *shapeNode->GetInfo().m_wireFrameOpenEdge;
					ndVector color1(m_showConcaveEdge ? ndVector(1.0f, 0.0f, 1.0f, 1.0f) : color);
					openEdgeMesh->SetColor(color1);
					openEdgeMesh->Render(this, matrix);
				}
			}
			else
			{
				ndFlatShadedDebugMesh* const mesh = *shapeNode->GetInfo().m_flatShaded;
				mesh->SetColor(color);
				mesh->Render(this, matrix);
			}
		}
	}
	
	RenderParticles(this);
}

void ndAuthor::RenderScene()
{
	D_TRACKTIME();
	ndFloat32 timestep = dGetElapsedSeconds();	
	CalculateFPS(timestep);
	UpdatePhysics(timestep);
	
	// Get the interpolated location of each body in the scene
	ndFloat32 interpolateParam = CalculateInteplationParam();
	m_cameraManager->InterpolateMatrices (this, interpolateParam);

	ImGuiIO& io = ImGui::GetIO();
	ndInt32 display_w = (ndInt32)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	ndInt32 display_h = (ndInt32)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	glViewport(0, 0, display_w, display_h);
	glScissor(0, 0, display_w, display_h);
	glEnable(GL_SCISSOR_TEST);	

	// Culling. 
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
	glEnable (GL_CULL_FACE);

	//	glEnable(GL_DITHER);
	// z buffer test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	// Setup camera matrix
	m_cameraManager->GetCamera()->SetViewMatrix(display_w, display_h);

	// render all entities
	const ndMatrix globalMatrix (ndGetIdentityMatrix());
	if (m_hideVisualMeshes) 
	{
		if (m_sky) 
		{
			m_sky->Render(timestep, this, globalMatrix);
		}
	} 
	else 
	{
		for (ndNode* node = ndList<ndDemoEntity*>::GetFirst(); node; node = node->GetNext()) 
		{
			ndDemoEntity* const entity = node->GetInfo();
			entity->Render(timestep, this, globalMatrix);
		}

		while (m_transparentHeap.GetCount()) 
		{
			const TransparentMesh& transparentMesh = m_transparentHeap[0];
			transparentMesh.m_mesh->RenderTransparency(this, transparentMesh.m_matrix);
			m_transparentHeap.Pop();
		}
	}

	if (m_showMeshSkeleton)
	{
		for (ndNode* node = ndList<ndDemoEntity*>::GetFirst(); node; node = node->GetNext())
		{
			ndDemoEntity* const entity = node->GetInfo();
			entity->RenderSkeleton(this, globalMatrix);
		}
	}

	if (m_showContactPoints)
	{
		m_world->Sync();
		RenderContactPoints(this);
	}
	
	if (m_showAABB) 
	{
		m_world->Sync();
		RenderBodiesAABB(this);
	}

	if (m_showScene)
	{
		m_world->Sync();
		RenderWorldScene(this);
	}

	//if (m_showRaycastHit) {
	//	RenderRayCastHit(m_world);
	//}

	if (m_showJointDebugInfo) 
	{
		m_world->Sync();
		RenderJointsDebugInfo(this);
	}

	if (m_showModelsDebugInfo)
	{
		m_world->Sync();
		RenderModelsDebugInfo(this);
	}

	if (m_showBodyFrame)
	{
		m_world->Sync();
		RenderBodyFrame(this);
	}

	if (m_showCenterOfMass) 
	{
		m_world->Sync();
		RenderCenterOfMass(this);
	}

	if (m_showNormalForces) 
	{
		m_world->Sync();
		RenderContactPoints(this);
		RenderNormalForces (this);
	}

	if (m_collisionDisplayMode)
	{
		m_world->Sync();
		DrawDebugShapes();
	}
}

void ndAuthor::TestImGui()
{
	// Main loop
	bool show_demo_window = true;
	bool show_another_window = false;

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if (1)
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

void ndAuthor::Run()
{
	// Main loop
	ndFloatExceptions exception;
	while (!glfwWindowShouldClose(m_mainFrame))
	{
		if (m_profilerMode)
		{
			ToggleProfiler();
			m_profilerMode = false;
		}

		m_suspendPhysicsUpdate = false;
		D_TRACKTIME();

		BeginFrame();
		RenderStats();

		//TestImGui();

		// Rendering
		ImGui::Render();
		RenderScene(ImGui::GetDrawData());

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_mainFrame);
	}
}
#endif


ndAuthor::ndAuthor()
{

}

ndAuthor::~ndAuthor()
{
}

void ndAuthor::Run()
{
}