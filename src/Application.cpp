// Custom Headers
#include <Renderer.h>	// Renderer header
#include <Utils.h>		// Utility header
#include <GUI.h>		// GUI header
#include <FileSystem.h> // Filesystem header
#include <Shader.h>		// Shader header
#include <Texture.h>	// Texture header
#include <CustomMath.h> // Math header
#include <Primitive.h>	// Primitive header
#include <Camera.h>		// Camera header
#include <Light.h>		// Light header
#include <Model.h>		// Model header

// Standard Headers
#include <iostream>
#include <vector>

// Const Settings
const int majorVersion = 4;
const int minorVersion = 6;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *windowTitle = "OpenGL window";

// Main Function
int main()
{
	Renderer renderer(majorVersion, minorVersion, SCR_WIDTH, SCR_HEIGHT);
	renderer.SetupGLFW();
	renderer.CreateWindow(windowTitle);
	if (renderer.window == NULL)
	{
		renderer.TerminateGLFW();
		return -1;
	}
	renderer.SetData();
	if (!renderer.CheckGLAD())
	{
		renderer.TerminateGLFW();
		return -1;
	}
	renderer.SetOtherData();
	renderer.SetupFrameBuffer();

	// Camera Settings
	glm::vec3 cameraPos(0.0f, 0.0f, 5.0f);
	Camera localCam(cameraPos);
	renderer.SetCamera(localCam);

	// Setup ImGui
	GUI gui(renderer.window, majorVersion, minorVersion);
	GUIWindow standardUI("UI window");
	GUIWindow cameraUI("Camera UI");
	GUIWindow primitiveUI("Primitive UI");
	GUIWindow objectUI("Object UI");

	Primitive triangle(FileSystem::getPath("resources/primitives/2D/triangle.2d").c_str());
	Primitive cube(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());
	Primitive lightObject(FileSystem::getPath("resources/primitives/3D/cube.3d").c_str());
	Primitive plane(FileSystem::getPath("resources/primitives/3D/plane.3d").c_str());
	Primitive quad(FileSystem::getPath("resources/primitives/3D/quad.3d").c_str());
	Primitive quad2D(FileSystem::getPath("resources/primitives/2D/quad.2d").c_str());
	Primitive skybox(FileSystem::getPath("resources/primitives/3D/cubemap.3d").c_str());

	Model mainModel(FileSystem::getPath("resources/models/backpack/backpack.obj"), true);

	Shader shader2D((FileSystem::getPath("shaders/modern/shader_2d.vs")).c_str(),
					(FileSystem::getPath("shaders/modern/shader_2d.fs")).c_str());
	Shader shader3D((FileSystem::getPath("shaders/modern/shader_scene.vs")).c_str(),
					(FileSystem::getPath("shaders/modern/shader_scene.fs")).c_str());
	Shader sourceShader(FileSystem::getPath("shaders/modern/shader_source.vs").c_str(),
						FileSystem::getPath("shaders/modern/shader_source.fs").c_str());
	Shader modelShader((FileSystem::getPath("shaders/modern/shader_scene.vs")).c_str(),
					   (FileSystem::getPath("shaders/modern/shader_scene.fs")).c_str());
	Shader planeShader((FileSystem::getPath("shaders/modern/shader_scene.vs")).c_str(),
					   (FileSystem::getPath("shaders/modern/shader_scene.fs")).c_str());
	Shader outlineShader((FileSystem::getPath("shaders/modern/shader_scene.vs")).c_str(),
						 (FileSystem::getPath("shaders/other/shader_outline.fs")).c_str());
	Shader transparentShader((FileSystem::getPath("shaders/modern/shader_scene.vs")).c_str(),
							 (FileSystem::getPath("shaders/modern/shader_transparent.fs")).c_str());
	Shader shaderFrame((FileSystem::getPath("shaders/modern/shader_2d.vs")).c_str(),
					   (FileSystem::getPath("shaders/modern/shader_frame.fs")).c_str());
	Shader skyboxShader((FileSystem::getPath("shaders/modern/shader_skybox.vs")).c_str(),
						(FileSystem::getPath("shaders/modern/shader_skybox.fs")).c_str());
	Shader reflectShader((FileSystem::getPath("shaders/modern/shader_scene.vs")).c_str(),
						 (FileSystem::getPath("shaders/modern/shader_reflect.fs")).c_str());

	vector<Texture> textures2D;
	Texture mainTex;
	mainTex.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/awesomeface.png").c_str());
	mainTex.type = "texture_diffuse";
	textures2D.push_back(mainTex);
	triangle.SetupTextures(textures2D);

	vector<Texture> textures3D, textures3DGamma;
	Texture diffuse, specular, emmision, diffuseG, emmisionG;
	diffuse.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/container2.png").c_str());
	diffuse.type = "texture_diffuse";
	textures3D.push_back(diffuse);
	specular.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
	specular.type = "texture_specular";
	textures3D.push_back(specular);
	emmision.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/matrix.jpg").c_str());
	emmision.type = "texture_emmision";
	textures3D.push_back(emmision);
	cube.SetupTextures(textures3D);

	diffuseG.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/container2.png").c_str(),
									  true, true);
	diffuseG.type = "texture_diffuse";
	textures3DGamma.push_back(diffuseG);
	textures3DGamma.push_back(specular);
	emmisionG.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/matrix.jpg").c_str(),
									   true, true);
	emmisionG.type = "texture_emmision";
	textures3DGamma.push_back(emmisionG);

	vector<Texture> planeTextures, planeTexturesGamma;
	Texture marble, marbleGamma;
	marble.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/marble.jpg").c_str());
	marble.type = "texture_diffuse";
	planeTextures.push_back(marble);
	plane.SetupTextures(planeTextures);
	marbleGamma.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/marble.jpg").c_str(),
										 true, true);
	marbleGamma.type = "texture_diffuse";
	planeTexturesGamma.push_back(marbleGamma);

	vector<Texture> quadTextures, windowTextures;
	Texture grass, windowTex;
	grass.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/grass.png").c_str(),
								   true, true, true);
	grass.type = "texture_diffuse";
	quadTextures.push_back(grass);
	windowTex.id = LoadTextureFromPath(FileSystem::getPath("resources/textures/window.png").c_str(),
									   true, true, true);
	windowTex.type = "texture_diffuse";
	windowTextures.push_back(windowTex);
	quad.SetupTextures(quadTextures);

	Texture skyboxTex;
	skyboxTex.id = LoadCubemapFromPath(FileSystem::getPath("resources/textures/skybox"), ".jpg");

	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, 3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f), glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f), glm::vec3(-1.3f, 1.0f, -1.5f)};

	glm::vec3 planePosition(0.0f, -2.0f, -5.0f);

	glm::vec3 quadPositions[] = {
		glm::vec3(-1.5f, 0.0f, -0.48f), glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)};

	PointLight pointLights[4];
	DirectionalLight dirLights[2];
	SpotLight spotLights[1];

	// Values setup
	bool renderGui = true;
	float angleVal = 0.0f;

	// UI Data
	bool showTriangles = false;
	bool showCubes = false;
	bool colorCubes = false;

	// Input Values
	bool canMoveCamera = false;
	bool canRotateCamera = false;
	bool isOrtho = false;
	float orthoSize = 3.0f;

	ImVec4 backgroundColor(0.0f, 0.0f, 0.05f, 1.0f);
	ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 cubeColor = ImVec4(objectColor.x, objectColor.y, objectColor.z, 1.0f);
	ImVec4 cubeOutlineColor = ImVec4(0.1f, 0.1, 0.3f, 1.0f);
	ImVec4 skyTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	float outlineWidth = 0.1f;
	bool drawOutline = false;
	bool toRefract = false;
	bool toReflect = false;

	int drawOption = 2;
	const char *drawComboItems[] = {
		"Draw Line", "Draw Point", "Draw Fill"};
	bool showFrameRate = false;

	// Scene Light Data
	bool useDirLight = true;
	bool usePointLight = true;
	bool useSpotLight = true;

	float cubeAmbient = 0.1;
	float cubeDiffuse = 1.0f;
	float cubeSpecular = 0.5f;
	float cubeShininess = 64;

	float lightAmbientPoint = 0.05f;
	float lightDiffusePoint = 0.125f;
	float lightSpecularPoint = 0.65f;
	float pointLightConstant = 1.0f;
	float pointLightLinear = 0.75f;
	float pointLightQuadratic = 0.2f;

	float lightAmbientDir = 0.025f;
	float lightDiffuseDir = 0.10f;
	float lightSpecularDir = 0.45f;
	glm::vec3 direction1(0.5f, -1.0f, -1.0f);
	glm::vec3 direction2(-0.3f, 0.8f, 1.0f);

	float lightAmbientSpot = 0.025f;
	float lightDiffuseSpot = 0.25f;
	float lightSpecularSpot = 0.45f;
	float spotLightCutoff = 12.5f;
	float spotLightOuterCutoff = 20.0f;

	Transform objectTransform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	bool globalRotation = false;

	float planeShininess = 2.0f;
	bool useBlinnModel = true;
	bool gammaCorrection = true;

	float grassHeight = -0.5f;
	bool isWindow = true;
	bool lockFrameRate = true;
	bool toCullFaces = true;
	int imageFilter = 0;
	const char *imageFliterOptions[] = {
		"Normal", "Invert Colors",
		"GrayScale", "Sharpen",
		"Blur", "Edge Detection",
		"Emboss", "Checkbox"};
	float checkboxSize = 16;

	standardUI.AddGUI(GUI_LINE, "Setup Standard Data:-", true);
	standardUI.AddGUI(GUI_COLOR, "Background Color", true, true, &backgroundColor);
	standardUI.AddGUI(GUI_COMBO, "Draw Mode", true, true, &drawOption, drawComboItems, 3);
	standardUI.AddGUI(GUI_COLOR, "Light Color", true, true, &lightColor);
	standardUI.AddGUI(GUI_COLOR, "Sky Color", true, true, &skyTint);
	standardUI.AddGUI(GUI_CHECKBOX, "Use Directional Light", true, &useDirLight);
	standardUI.AddGUI(GUI_FLOAT, "Light Ambient##1", true, &lightAmbientDir, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Light Diffuse##1", true, &lightDiffuseDir, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Light Specular##1", true, &lightSpecularDir, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_VECTOR3, "Direction 1", true, &direction1.x, -1.0f, 1.0f);
	standardUI.AddGUI(GUI_VECTOR3, "Direction 2", true, &direction2.x, -1.0f, 1.0f);
	standardUI.AddGUI(GUI_CHECKBOX, "Use Point Light", true, &usePointLight);
	standardUI.AddGUI(GUI_FLOAT, "Light Rotation##2", true, &angleVal, 0.0f, 360.0f);
	standardUI.AddGUI(GUI_FLOAT, "Light Ambient##2", true, &lightAmbientPoint, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Light Diffuse##2", true, &lightDiffusePoint, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Light Specular", true, &lightSpecularPoint, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Constant", true, &pointLightConstant, 0.0f, 1.0f);
	standardUI.AddGUI(GUI_FLOAT, "Linear", true, &pointLightLinear, 0.0f, 1.0f);
	standardUI.AddGUI(GUI_FLOAT, "Quadratic", true, &pointLightQuadratic, 0.0f, 1.0f);
	standardUI.AddGUI(GUI_CHECKBOX, "Use Spot Light", true, &useSpotLight);
	standardUI.AddGUI(GUI_FLOAT, "Light Ambient##3", true, &lightAmbientSpot, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Light Diffuse##3", true, &lightDiffuseSpot, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Light Specular##3", true, &lightSpecularSpot, 0.0f, 2.0f);
	standardUI.AddGUI(GUI_FLOAT, "Cutoff", true, &spotLightCutoff, 0.0f, 20.0f);
	standardUI.AddGUI(GUI_FLOAT, "OuterCutoff", true, &spotLightOuterCutoff, spotLightCutoff, 45.0f);
	standardUI.AddGUI(GUI_CHECKBOX, "Use Blinn Model", true, &useBlinnModel);
	standardUI.AddGUI(GUI_CHECKBOX, "Use Gamma Correction", true, &gammaCorrection);
	standardUI.AddGUI(GUI_CHECKBOX, "Lock FrameRate", true, &lockFrameRate);
	standardUI.AddGUI(GUI_CHECKBOX, "Face Culling", true, &toCullFaces);

	cameraUI.AddGUI(GUI_LINE, "Setup Camera Data:-", true);
	cameraUI.AddGUI(GUI_CHECKBOX, "Move Camera", true, &canMoveCamera);
	cameraUI.AddGUI(GUI_CHECKBOX, "Rotate Camera", true, &canRotateCamera);
	cameraUI.AddGUI(GUI_CHECKBOX, "Enable Orthographic", true, &isOrtho);
	cameraUI.AddGUI(GUI_FLOAT, "Camera Size", true, &orthoSize, 1.0f, 10.0f);

	primitiveUI.AddGUI(GUI_LINE, "Setup Primitive Data:-", true);
	primitiveUI.AddGUI(GUI_CHECKBOX, "Show Triangles", true, &showTriangles);
	primitiveUI.AddGUI(GUI_CHECKBOX, "Show Cubes", true, &showCubes);
	primitiveUI.AddGUI(GUI_CHECKBOX, "Color Cubes", true, &colorCubes);
	primitiveUI.AddGUI(GUI_COLOR, "Cube Color", true, true, &cubeColor);
	primitiveUI.AddGUI(GUI_FLOAT, "Cube Ambient", true, &cubeAmbient, 0.0f, 2.0f);
	primitiveUI.AddGUI(GUI_FLOAT, "Cube Diffuse", true, &cubeDiffuse, 0.0f, 2.0f);
	primitiveUI.AddGUI(GUI_FLOAT, "Cube Specular", true, &cubeSpecular, 0.0f, 2.0f);
	primitiveUI.AddGUI(GUI_FLOAT, "Cube Shininess", true, &cubeShininess, 0.0f, 256.0f);
	primitiveUI.AddGUI(GUI_CHECKBOX, "Show Outline", true, &drawOutline);
	primitiveUI.AddGUI(GUI_COLOR, "Cube Outline", true, true, &cubeOutlineColor);
	primitiveUI.AddGUI(GUI_FLOAT, "Outline Width", true, &outlineWidth, 0.0f, 1.0f);
	primitiveUI.AddGUI(GUI_FLOAT, "Plane Shininess", true, &planeShininess, 0.01f, 64.0f);

	objectUI.AddGUI(GUI_LINE, "Setup Model Data:-", true);
	objectUI.AddGUI(GUI_CHECKBOX, "Global Rotation", true, &globalRotation);
	objectUI.AddGUI(GUI_LINE, "Set Transform:-", true);
	objectUI.AddGUI(GUI_VECTOR3, "Position", true, &objectTransform.position.x, -5.0f, 5.0f);
	objectUI.AddGUI(GUI_VECTOR3, "Rotation", true, &objectTransform.rotation.x, -360.0f, 360.0f);
	objectUI.AddGUI(GUI_VECTOR3, "Scale", true, &objectTransform.scale.x, 0.01f, 5.0f);

	objectUI.AddGUI(GUI_FLOAT, "Grass Height", true, &grassHeight, -5.0f, 5.0f);
	objectUI.AddGUI(GUI_CHECKBOX, "Draw Window", true, &isWindow);
	objectUI.AddGUI(GUI_CHECKBOX, "To Reflect", true, &toReflect);
	objectUI.AddGUI(GUI_CHECKBOX, "To Refract", true, &toRefract);

	renderer.StartTimer();
	while (!renderer.CheckWindowFlag())
	{
		// New Frame
		gui.NewFrame();
		renderer.NewFrame();

		renderer.frameBuffer.BindFBO();
		glEnable(GL_DEPTH_TEST);
		renderer.frameBuffer.RefreshRBO((int)renderer.GetCurrentWidth(),
										(int)renderer.GetCurrentHeight());
		renderer.frameBuffer.RefreshTexture((int)renderer.GetCurrentWidth(),
											(int)renderer.GetCurrentHeight());

		// Process Data
		renderer.ProcessInput(canMoveCamera);
		renderer.SetColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
		renderer.SetDraw(drawOption);
		renderer.ProcessMouse(canRotateCamera);
		if (renderer.CheckInput(GLFW_KEY_SPACE))
		{
			renderGui = false;
			canRotateCamera = true;
			canMoveCamera = true;
		}
		if (renderer.CheckInput(GLFW_KEY_LEFT_ALT))
		{
			renderGui = true;
			canRotateCamera = false;
			canMoveCamera = false;
		}
		if (toCullFaces)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		// View matrix :: WORLD TO VIEW
		glm::mat4 view;
		view = (*(renderer.GetCamera())).GetViewMatrix();

		// Projection matrix:: VIEW TO CLIPPED
		glm::mat4 projection;
		if (!isOrtho)
		{
			// FOV = X degrees, RATIO =WIDTH/HEIGHT, Near Plane = 0.1, Far Plane = 100.0
			projection = glm::perspective(glm::radians(renderer.GetZoom()),
										  ((float)renderer.GetCurrentWidth()) / ((float)renderer.GetCurrentHeight()),
										  0.1f, 100.0f);
		}
		else
		{
			float aspectRatio = renderer.GetCurrentWidth() / renderer.GetCurrentHeight();
			float cWidth = aspectRatio * orthoSize;
			float cHeight = orthoSize;
			projection = glm::ortho(-cWidth / 2.0f, cWidth / 2.0f, -cHeight / 2.0f, cHeight / 2.0f,
									0.01f, 100.0f);
		}

		// Setups Lights
		glm::vec3 currentLightColor(lightColor.x, lightColor.y, lightColor.z);
		for (int i = 0; i < 4; i++)
		{
			pointLights[i].position = pointLightPositions[i];
			pointLights[i].ambient = currentLightColor * lightAmbientPoint;
			pointLights[i].diffuse = currentLightColor * lightDiffusePoint;
			pointLights[i].specular = currentLightColor * lightSpecularPoint;
			pointLights[i].constant = pointLightConstant;
			pointLights[i].linear = pointLightLinear;
			pointLights[i].quadratic = pointLightQuadratic;
		}
		for (int i = 0; i < 2; i++)
		{
			dirLights[i].ambient = currentLightColor * lightAmbientDir;
			dirLights[i].diffuse = currentLightColor * lightDiffuseDir;
			dirLights[i].specular = currentLightColor * lightSpecularDir;
		}
		dirLights[0].direction = direction1;
		dirLights[1].direction = direction2;
		for (int i = 0; i < 1; i++)
		{
			spotLights[i].ambient = currentLightColor * lightAmbientSpot;
			spotLights[i].diffuse = currentLightColor * lightDiffuseSpot;
			spotLights[i].specular = currentLightColor * lightSpecularSpot;
			spotLights[i].position = (*(renderer.GetCamera())).Position;
			spotLights[i].direction = (*(renderer.GetCamera())).Front;
			spotLights[i].cutoff = spotLightCutoff;
			spotLights[i].outerCutoff = spotLightOuterCutoff;
		}
		// Render Objects
		if (drawOutline)
		{
			glStencilMask(0x00);
		}
		// Draw Plane
		if (gammaCorrection)
		{
			plane.SetupTextures(planeTexturesGamma);
		}
		else
		{
			plane.SetupTextures(planeTextures);
		}
		glm::mat4 planeModel(1.0f);
		planeModel = glm::translate(planeModel, planePosition);
		planeModel = glm::scale(planeModel, glm::vec3(2.0f));
		planeShader.use();
		planeShader.setBool("useTexture", true);
		planeShader.setFloat("material.shininess", planeShininess);
		planeShader.SetPointLights(pointLights, 4, angleVal);
		planeShader.SetDirLights(dirLights, 2);
		planeShader.SetSpotLights(spotLights, 1);
		planeShader.setBool("useDirLight", useDirLight);
		planeShader.setBool("usePointLight", usePointLight);
		planeShader.setBool("useSpotLight", useSpotLight);
		planeShader.setBool("useBlinnModel", useBlinnModel);
		planeShader.setBool("correctGamma", gammaCorrection);
		planeShader.setVec3("viewPos", (*(renderer.GetCamera())).Position);
		planeShader.SetMatrices(planeModel, view, projection);
		plane.Draw(planeShader);

		// First Render pass
		if (drawOutline)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}
		if (showCubes)
		{
			if (gammaCorrection)
			{
				cube.SetupTextures(textures3DGamma);
			}
			else
			{
				cube.SetupTextures(textures3D);
			}
			shader3D.use();
			if (colorCubes)
			{
				glm::vec3 currentCubeColor(cubeColor.x, cubeColor.y, cubeColor.z);
				shader3D.SetMaterial(currentCubeColor * cubeAmbient,
									 currentCubeColor * cubeDiffuse,
									 currentCubeColor * cubeSpecular, cubeShininess);
				shader3D.setBool("useTexture", false);
			}
			else
			{
				shader3D.setBool("useTexture", true);
			}
			shader3D.setFloat("material.shininess", cubeShininess);
			shader3D.SetPointLights(pointLights, 4, angleVal);
			shader3D.SetDirLights(dirLights, 2);
			shader3D.SetSpotLights(spotLights, 1);
			shader3D.setBool("useDirLight", useDirLight);
			shader3D.setBool("usePointLight", usePointLight);
			shader3D.setBool("useSpotLight", useSpotLight);
			shader3D.setBool("useBlinnModel", useBlinnModel);
			shader3D.setBool("correctGamma", gammaCorrection);
			shader3D.setVec3("viewPos", (*(renderer.GetCamera())).Position);
			for (unsigned int i = 0; i < 10; i++)
			{
				// model matrix :: LOCAL TO WORLD
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 15.0f * (i);
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader3D.SetMatrices(model, view, projection);
				cube.Draw(shader3D);
			}
		}
		// Second Render Pass
		if (drawOutline)
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			if (showCubes)
			{
				float scale = 1.0f + outlineWidth;
				outlineShader.use();
				outlineShader.setVec3("outlineColor",
									  glm::vec3(cubeOutlineColor.x, cubeOutlineColor.y, cubeOutlineColor.z));
				for (unsigned int i = 0; i < 10; i++)
				{
					// model matrix :: LOCAL TO WORLD
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, cubePositions[i]);
					float angle = 15.0f * (i);
					model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
					model = glm::scale(model, glm::vec3(scale));
					outlineShader.SetMatrices(model, view, projection);
					cube.Draw(outlineShader);
				}
			}
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glEnable(GL_DEPTH_TEST);
		}

		// Draw Lights
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
		sourceShader.use();
		sourceShader.setVec3("sourceColor", currentLightColor);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model = lightModel;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			sourceShader.SetMatrices(model, view, projection);
			lightObject.Draw(sourceShader);
		}

		// Draw Model
		// Setup matrices
		glm::vec3 objectPos(objectTransform.position);
		glm::vec3 objectRot(objectTransform.rotation);
		glm::vec3 objectScale(objectTransform.scale);
		glm::mat4 model(1.0f);
		model = glm::translate(model, objectPos);
		if (!globalRotation)
		{
			model = glm::rotate(model, glm::radians(objectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(objectRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(objectRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else
		{
			glm::mat4 modelY = glm::rotate(model, glm::radians(objectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 modelZ = glm::rotate(model, glm::radians(objectRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 modelX = glm::rotate(model, glm::radians(objectRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = (modelX * (modelZ * (modelY * model)));
		}
		model = glm::scale(model, objectScale);

		modelShader.use();
		modelShader.setBool("useTexture", true);
		modelShader.setFloat("material.shininess", cubeShininess);
		modelShader.SetPointLights(pointLights, 4, angleVal);
		modelShader.SetDirLights(dirLights, 2);
		modelShader.SetSpotLights(spotLights, 1);
		modelShader.setBool("useDirLight", useDirLight);
		modelShader.setBool("usePointLight", usePointLight);
		modelShader.setBool("useSpotLight", useSpotLight);
		modelShader.setBool("useBlinnModel", useBlinnModel);
		modelShader.setBool("correctGamma", gammaCorrection);
		modelShader.setVec3("viewPos", (*(renderer.GetCamera())).Position);
		modelShader.SetMatrices(model, view, projection);
		modelShader.setFloat("material.shininess", 64);
		if (toReflect || toRefract)
		{
			reflectShader.use();
			reflectShader.SetMatrices(model, view, projection);
			reflectShader.setVec3("cameraPos", (*(renderer.GetCamera())).Position);
			reflectShader.setBool("toRefract", toRefract);
			BindCubemap(skyboxTex.id);
			mainModel.Draw(reflectShader);
			BindCubemap(0);
		}
		else
		{
			mainModel.Draw(modelShader);
		}

		transparentShader.use();
		transparentShader.setBool("toDiscard", !isWindow);
		if (isWindow)
		{
			quad.SetupTextures(windowTextures);
		}
		else
		{
			quad.SetupTextures(quadTextures);
		}
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < 5; i++)
		{
			float distance = glm::length((*(renderer.GetCamera())).Position - quadPositions[i]);
			sorted[distance] = quadPositions[i];
		}

		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin();
			 it != sorted.rend();
			 ++it)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, it->second);
			model = glm::translate(model, glm::vec3(0.0f, planePosition.y + grassHeight, 0.0f));
			model = glm::scale(model, glm::vec3(0.25f));
			transparentShader.SetMatrices(model, view, projection);
			quad.Draw(transparentShader);
		}

		// Draw Triangle
		if (showTriangles)
		{
			shader2D.use();
			triangle.Draw(shader2D);
		}

		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		glm::mat4 newView = glm::mat4(glm::mat3(view));
		glm::mat4 newProjection = projection;
		if (isOrtho)
		{
			newProjection = glm::perspective(glm::radians(renderer.GetZoom()),
											 ((float)renderer.GetCurrentWidth()) / ((float)renderer.GetCurrentHeight()),
											 0.1f, 100.0f);
		}
		skyboxShader.setMat4("projection", newProjection);
		skyboxShader.setMat4("view", newView);
		skyboxShader.setVec3("tintColor", glm::vec3(skyTint.x, skyTint.y, skyTint.z));
		BindCubemap(skyboxTex.id);
		skybox.Draw(skyboxShader);
		glDepthFunc(GL_LESS);

		// Second pass
		renderer.frameBuffer.UnBindFBO(); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		renderer.SetDraw();

		shaderFrame.use();
		vector<Texture> frameTextures;
		Texture frameTex;
		frameTex.id = renderer.frameBuffer.textureColorBuffer;
		frameTex.type = "texture_diffuse";
		frameTextures.push_back(frameTex);
		quad2D.SetupTextures(frameTextures);
		shaderFrame.setInt("filterChoice", imageFilter);
		shaderFrame.setInt("width", (int)renderer.GetCurrentWidth());
		shaderFrame.setInt("height", (int)renderer.GetCurrentHeight());
		shaderFrame.setInt("boxSize", (int)checkboxSize);
		quad2D.Draw(shaderFrame);

		// Set UI
		if (renderGui)
		{
			standardUI.ShowGUI();
			ImGui::Checkbox("Show FrameRate", &showFrameRate);
			if (showFrameRate)
			{
				ImGui::SameLine();
				int val = (int)(1.0f / renderer.deltaTime);
				ImGui::Text("FrameRate:- %d", val);
			}
			standardUI.EndGUI();
			cameraUI.ShowGUI();
			ImGui::Combo("Image Filters", &imageFilter, imageFliterOptions, 8);
			ImGui::DragFloat("CheckBoxSize", &checkboxSize, 1, 1, 64);
			cameraUI.EndGUI();
			primitiveUI.ShowGUI();
			primitiveUI.EndGUI();
			objectUI.ShowGUI();
			if (ImGui::Button("Reset Object"))
			{
				objectTransform.ResetToOrigin();
			}
			objectUI.EndGUI();
		}

		// call events
		gui.RenderGUI();
		renderer.SwapBuffers(lockFrameRate);
	}
	// Free Shaders
	shader2D.FreeData();
	shader3D.FreeData();
	sourceShader.FreeData();
	outlineShader.FreeData();
	modelShader.FreeData();
	planeShader.FreeData();
	transparentShader.FreeData();
	shaderFrame.FreeData();
	skyboxShader.FreeData();
	reflectShader.FreeData();

	// Free Vertex Arrays
	triangle.vertexArray.FreeData();
	cube.vertexArray.FreeData();
	lightObject.vertexArray.FreeData();
	plane.vertexArray.FreeData();
	mainModel.FreeData();
	quad.vertexArray.FreeData();
	quad2D.vertexArray.FreeData();
	skybox.vertexArray.FreeData();

	// Free frame buffer
	renderer.frameBuffer.FreeFBO();
	renderer.frameBuffer.FreeRBO();

	// Terminate Program
	gui.TerminateGUI();
	renderer.TerminateGLFW();
	return 0;
}