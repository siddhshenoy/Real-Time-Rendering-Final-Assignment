#include <spdlog/spdlog.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#define			IMGUI_GLSL_VERSION			"#version 130"

#include <map>	

#include <Core/Window.h>
#include <Rendering/Skybox.h>
#include <Rendering/Light.h>
#include <Resource/ResourceManager.h>
#include <Structures/Map.h>
namespace Main {
	Map<int, bool> KeyMap;

	Texture* TeapotTexture;

	Light* LightObject;

	typedef struct TeapotObject {
		bool ApplyTexture;
		Object* Teapot;
		Shader* TeapotShader;
		Material* TeapotMat;
	} TeapotObject;

	TeapotObject GlobalObjects[5];

	typedef struct EngineLightObject {
		bool ApplyTexture;
		Object* LightObject;
		Shader* LightShader;
		Material* LightMaterial;
	}
	EngineLightObject;
	EngineLightObject ELightObject;

	typedef struct CookTorranceLightData {
		float F0 = 0.8f;
		float Roughness = 0.1f;
		float k = 0.2f;
	}
	CookTorranceLightData;
	CookTorranceLightData CTLightData;

	typedef struct MinnaertLightData {
		float k = 0.5f;
	};
	MinnaertLightData;
	MinnaertLightData MinLightData;

	typedef struct OrenNayarLightData {
		float Roughness = 1.0f;
		float Albedo = 1.0f;
		float Irradiance = 1.0f;
	}
	OrenNayarLightData;
	OrenNayarLightData ONLightData;

	typedef struct TransmittanceData {
		float ETA = 0.66f;
		glm::vec3 ETARGB = glm::vec3(0.65f, 0.67f, 0.69f);
		float IOR = 2.66;
		float FresnelPower = 5.0f;
	}
	TransmittanceData;
	TransmittanceData Transmittance;

	Texture* CheckeredTexture;
	Texture* TileTexture0;
	Texture* TileTexture0Normal;
	typedef struct TextureMinMag {
		GLuint minType;
		GLuint magType;
		GLfloat LODbias = 0.0f;

	}
	TextureMinMag;
	TextureMinMag MinMagProperty;

	Camera* GlobalCamera;
	Skybox* GlobalSkybox;
	Skybox* CurrentSkybox;
	Skybox* Skybox1;
	float fRoughness = 1.0f;
	float fAlbedo = 1.0f;
	float fIrradiance = 1.0f;
	int fIntensityMultiplier = 1;

	float fBumpStrength = 2.0f;
	float fTexMultiplier = 1.f;
	bool ApplyTextures = true;
	bool fFlipTangentCalculation = false;
	bool fFlipBitangentCross = false;
	bool fFlipNormalInMatrix = false;
	bool fFlipTangent = false;
	bool fApplyMainTexture = true;
	bool fApplyNormalTexture = true;
	bool ShouldObjectsRotate = false;

	void InitFunction();
	void RenderingFunction(GLFWwindow* window);
	void InitializeImGui();
	void RenderImGui();
	void EnableImGUIDockspace();
	void RenderObjectListTable(Object* object);
	void ProcessInputs();
	Window* gWindow;

	void WindowMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	void WindowKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	int min_types[] = {
		GL_NEAREST,
		GL_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_LINEAR
	};

	int mag_types[] = {
		GL_NEAREST,
		GL_LINEAR
	};

	std::map<std::string, Skybox*> SkyboxList;

	std::map<std::string, Material*> MaterialList;

	glm::vec3 CameraPositions[][2] = {
		{
			glm::vec3(0.0f, 2.5f, -5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		},
		{
			glm::vec3(3.0f, 1.5f, -2.5f),
			glm::vec3(3.0f, 0.0f, 0.0f)
		},
		{
			glm::vec3(0.0f, 1.5f, -2.5f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		},
		{
			glm::vec3(-3.0f, 1.5f, -2.5f),
			glm::vec3(-3.0f, 0.0f, 0.0f)
		},
		{
			glm::vec3(-3.0f, 1.5f, 0.5f),
			glm::vec3(-3.0f, 0.0f, 3.0f)
		},
		{
			glm::vec3(0.0f, 1.5f, 0.5f),
			glm::vec3(0.0f, 0.0f, 3.0f)
		}
	};

	int main() {
		gWindow = new Window();
		gWindow->Create("Real Time Rendering - Lab 3", 50, 50, 800, 600, true);
		//gWindow->SetWindowFavIcon("./GameResources/Icons/window_favicon.png");
		gWindow->SetRenderingFunction(RenderingFunction);
		gWindow->ShowWindow();
		gWindow->SetWindowKeyboardCallback(WindowKeyPressCallback);
		gWindow->SetWindowMouseCursorCallback(WindowMouseMoveCallback);
		/*gWindow->SetWindowMouseButtonCallback(WindowMouseClickCallback);
		gWindow->SetWindowMouseScrollCallback(WindowMouseScrollCallback);*/
		InitFunction();
		gWindow->RenderLoop();
		return 0;
	}

	void InitFunction() {

		gWindow->HideCursor();
		// Enable Depth Test
		glEnable(GL_DEPTH_TEST);

		// Enable back face culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		GlobalCamera = new Camera();

		GlobalCamera->SetProjectionType(ProjectionType::PERSPECTIVE);
		GlobalCamera->Translate(glm::vec3(0.0f, 0.0f, 10.0f));
		GlobalCamera->LookAt(glm::vec3(0.0f, 2.5f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		GlobalSkybox = new Skybox("./GameResources/Skybox/Brooklyn", "png");

		LightObject = new Light();
		LightObject->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
		LightObject->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

		TeapotTexture = new Texture();
		TeapotTexture->LoadTexture("./GameResources/Textures/Stone_Wall_007_SD/Stone_Wall_007_COLOR.jpg");

		Texture* NormalTexture = new Texture();
		NormalTexture->LoadTexture("./GameResources/Textures/Stone_Wall_007_SD/Stone_Wall_007_NORM.jpg");

		BaseShader* PhongVertexShader, * PhongFragmentShader;
		PhongVertexShader = new BaseShader("Phong Vertex Shader", "./GameResources/Shaders/PhongOriginalVertexShader.glsl", GL_VERTEX_SHADER);
		PhongFragmentShader = new BaseShader("Phong Fragment Shader", "./GameResources/Shaders/PhongOriginalFragmentShader.glsl", GL_FRAGMENT_SHADER);


		BaseShader* skyboxVS, * skyboxFS;
		skyboxVS = new BaseShader("Skybox Vertex Shader", "./GameResources/Shaders/SkyboxVS.glsl", GL_VERTEX_SHADER);
		skyboxFS = new BaseShader("Skybox Fragment Shader", "./GameResources/Shaders/SkyboxFS.glsl", GL_FRAGMENT_SHADER);
		Shader* SkyboxShader = new Shader(std::vector<BaseShader*>({ skyboxVS, skyboxFS }));
		GlobalSkybox->SetShader(SkyboxShader);

		Skybox1 = new Skybox("./GameResources/Skybox/Desert", "png");
		Skybox1->SetShader(SkyboxShader);

		CurrentSkybox = GlobalSkybox;




		GlobalObjects[1].TeapotShader = new Shader(std::vector<BaseShader*>({ PhongVertexShader,PhongFragmentShader }));
		Material* StoneWall = new Material();
		Texture* StoneTexture = new Texture();
		StoneTexture->LoadTexture("./GameResources/Textures/Stone_Wall_007_SD/Stone_Wall_007_COLOR.jpg");
		Texture* StoneNormalTexture = new Texture();
		StoneNormalTexture->LoadTexture("./GameResources/Textures/Stone_Wall_007_SD/Stone_Wall_007_NORM.jpg");
		StoneWall->AttachShader(GlobalObjects[1].TeapotShader);
		StoneWall->AttachTexture("Default Texture", StoneTexture, 0);
		StoneWall->AttachTexture("Normal Texture", StoneNormalTexture, 1);

		Material* BrickWall0 = new Material();
		Texture* BrickWall0Texture = new Texture();
		BrickWall0Texture->LoadTexture("./GameResources/Textures/Brick_Wall_010_SD/Brick_Wall_010_COLOR.jpg");
		Texture* BrickWall0NormalTexture = new Texture();
		BrickWall0NormalTexture->LoadTexture("./GameResources/Textures/Brick_Wall_010_SD/Brick_Wall_010_NORM.jpg");
		BrickWall0->AttachShader(GlobalObjects[1].TeapotShader);
		BrickWall0->AttachTexture("Default Texture", BrickWall0Texture, 0);
		BrickWall0->AttachTexture("Normal Texture", BrickWall0NormalTexture, 1);

		Material* BrickWall1 = new Material();
		Texture* BrickWall1Texture = new Texture();
		BrickWall1Texture->LoadTexture("./GameResources/Textures/Brick_Wall_015_SD/Brick_Wall_015_COLOR.jpg");
		Texture* BrickWall1NormalTexture = new Texture();
		BrickWall1NormalTexture->LoadTexture("./GameResources/Textures/Brick_Wall_015_SD/Brick_Wall_015_NORM.jpg");
		BrickWall1->AttachShader(GlobalObjects[1].TeapotShader);
		BrickWall1->AttachTexture("Default Texture", BrickWall1Texture, 0);
		BrickWall1->AttachTexture("Normal Texture", BrickWall1NormalTexture, 1);

		Material* WoodTexture0 = new Material();
		Texture* WoodTexture0Texture = new Texture();
		WoodTexture0Texture->LoadTexture("./GameResources/Textures/Wood_Herringbone_Tiles_004_SD/Substance_Graph_BaseColor.jpg");
		Texture* WoodTexture0NormalTexture = new Texture();
		WoodTexture0NormalTexture->LoadTexture("./GameResources/Textures/Wood_Herringbone_Tiles_004_SD/Substance_Graph_Normal.jpg");
		WoodTexture0->AttachShader(GlobalObjects[1].TeapotShader);
		WoodTexture0->AttachTexture("Default Texture", WoodTexture0Texture, 0);
		WoodTexture0->AttachTexture("Normal Texture", WoodTexture0NormalTexture, 1);

		Material* GravelTexture0 = new Material();
		Texture* GravelTexture0Texture = new Texture();
		GravelTexture0Texture->LoadTexture("./GameResources/Textures/Gravel_001_SD/Gravel_001_BaseColor.jpg");
		Texture* GravelTexture0NormalTexture = new Texture();
		GravelTexture0NormalTexture->LoadTexture("./GameResources/Textures/Gravel_001_SD/Gravel_001_Normal.jpg");
		GravelTexture0->AttachShader(GlobalObjects[1].TeapotShader);
		GravelTexture0->AttachTexture("Default Texture", GravelTexture0Texture, 0);
		GravelTexture0->AttachTexture("Normal Texture", GravelTexture0NormalTexture, 1);

		Material* PlaneMaterial = new Material();
		CheckeredTexture = new Texture();
		CheckeredTexture->LoadTexture("./GameResources/Textures/Pattern/CheckerBoard1.png");
		PlaneMaterial->AttachShader(GlobalObjects[1].TeapotShader);
		//PlaneMaterial->AttachTexture(CheckeredTexture);
		PlaneMaterial->AttachTexture("Default Texture", CheckeredTexture, 0);

		Material* TileMaterial = new Material();
		TileTexture0 = new Texture();
		TileTexture0->LoadTexture("./GameResources/Textures/Tiles_041_SD/Tiles_041_basecolor.jpg");
		TileTexture0Normal = new Texture();
		TileTexture0Normal->LoadTexture("./GameResources/Textures/Tiles_041_SD/Tiles_041_normal.jpg");
		TileMaterial->AttachShader(GlobalObjects[1].TeapotShader);
		TileMaterial->AttachTexture("Default Texture", TileTexture0, 0);
		TileMaterial->AttachTexture("Normal Texture", TileTexture0Normal, 1);


		MaterialList.insert(std::pair<std::string, Material*>("Stone Wall", StoneWall));
		MaterialList.insert(std::pair<std::string, Material*>("Brick Wall - 0", BrickWall0));
		MaterialList.insert(std::pair<std::string, Material*>("Brick Wall - 1", BrickWall1));
		MaterialList.insert(std::pair<std::string, Material*>("Wood Texture", WoodTexture0));
		MaterialList.insert(std::pair<std::string, Material*>("Gravel Texture", GravelTexture0));

		GlobalObjects[1].Teapot = ResourceManager::LoadObject("./GameResources/Models/Extra/Plane.fbx");
		GlobalObjects[1].Teapot->SetTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
		GlobalObjects[1].TeapotMat = new Material();
		GlobalObjects[1].TeapotMat->AttachShader(GlobalObjects[1].TeapotShader);
		GlobalObjects[1].Teapot->AssignMaterial(TileMaterial);
		GlobalObjects[1].ApplyTexture = true;

		GlobalObjects[2].Teapot = ResourceManager::LoadObject("./GameResources/Models/Extra/Cube.fbx");
		GlobalObjects[2].Teapot->SetTranslation(glm::vec3(0.0f, 1.0f, 0.0f));
		GlobalObjects[2].TeapotMat = new Material();
		GlobalObjects[2].TeapotMat->AttachShader(GlobalObjects[1].TeapotShader);
		GlobalObjects[2].TeapotMat->AttachTexture("Default Tex", CheckeredTexture, 0);
		GlobalObjects[2].Teapot->AssignMaterial(GlobalObjects[2].TeapotMat);
		GlobalObjects[2].ApplyTexture = true;


		BaseShader* LightVertexShader, * LightFragmentShader;
		LightVertexShader = new BaseShader("Light Vertex Shader", "./GameResources/Shaders/LightVertexShader.glsl", GL_VERTEX_SHADER);
		LightFragmentShader = new BaseShader("Light Fragment Shader", "./GameResources/Shaders/LightFragmentShader.glsl", GL_FRAGMENT_SHADER);
		Shader* LightShader = new Shader(std::vector<BaseShader*>({ LightVertexShader, LightFragmentShader }));

		ELightObject.LightObject = ResourceManager::LoadObject("./GameResources/Models/Extra/Cube.fbx");
		//ELightObject.LightObject->SetScale(glm::vec3(0.01f));
		ELightObject.LightShader = LightShader;
		ELightObject.LightMaterial = new Material();
		ELightObject.LightMaterial->AttachShader(ELightObject.LightShader);
		ELightObject.LightObject->AssignMaterial(ELightObject.LightMaterial);

		InitializeImGui();

	}
	float RefractiveIndex;

	void RenderingFunction(GLFWwindow* window) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


		int windowWidth, windowHeight;
		gWindow->GetDimensions(&windowWidth, &windowHeight);
		if (windowWidth > 0 && windowHeight > 0)
			GlobalCamera->InitializePerspectiveParameters(70.0f, ((float)windowWidth / (float)windowHeight), 0.001f, 1000.0f);

		CurrentSkybox->UseSkybox(GlobalCamera);
		for (Object* object : ResourceManager::gObjectList) {
			object->Render(GlobalCamera);
			object->GetMaterial()->GetShader()->SetUniform1i("Texture", 0);
			object->GetMaterial()->GetShader()->SetUniform1i("NormalMap", 1);
			object->GetMaterial()->GetShader()->SetUniform3f("fLightColor", LightObject->GetColor().x, LightObject->GetColor().y, LightObject->GetColor().z);
			object->GetMaterial()->GetShader()->SetUniform3f("fLightPosition", LightObject->GetPosition().x, LightObject->GetPosition().y, LightObject->GetPosition().z);
			object->GetMaterial()->GetShader()->SetUniform3f("fCameraPosition", GlobalCamera->GetTranslation().x, GlobalCamera->GetTranslation().y, GlobalCamera->GetTranslation().z);
			object->GetMaterial()->GetShader()->SetUniform1f("fAmbientStrength", LightObject->LightData.AmbientStrength);
			object->GetMaterial()->GetShader()->SetUniform1f("fDiffuseStrength", LightObject->LightData.DiffuseStrength);
			object->GetMaterial()->GetShader()->SetUniform1f("fSpecularStrength", LightObject->LightData.SpecularStrength);
			object->GetMaterial()->GetShader()->SetUniform1f("fSpecularPower", LightObject->LightData.SpecularPower);
			object->GetMaterial()->GetShader()->SetUniform1f("fRoughness", ONLightData.Roughness);
			object->GetMaterial()->GetShader()->SetUniform1f("fAlbedo", ONLightData.Albedo);
			object->GetMaterial()->GetShader()->SetUniform1f("fIrradiance", ONLightData.Irradiance);
			object->GetMaterial()->GetShader()->SetUniform1i("fApplyTexture", ApplyTextures ? 1 : 0);
			object->GetMaterial()->GetShader()->SetUniform1i("fIntensityMultiplier", fIntensityMultiplier);
			object->GetMaterial()->GetShader()->SetUniform1f("F0", CTLightData.F0);
			object->GetMaterial()->GetShader()->SetUniform1f("roughness", CTLightData.Roughness);
			object->GetMaterial()->GetShader()->SetUniform1f("k", CTLightData.k);
			object->GetMaterial()->GetShader()->SetUniform1f("minK", MinLightData.k);
			object->GetMaterial()->GetShader()->SetUniform3f("fObjectColor", 1.0f, 1.0f, 1.0f);
			object->GetMaterial()->GetShader()->SetUniform1i("fFlipTangentCalculation", fFlipTangentCalculation ? 1 : 0);
			object->GetMaterial()->GetShader()->SetUniform1i("fFlipBitangentCross", fFlipBitangentCross ? 1 : 0);
			object->GetMaterial()->GetShader()->SetUniform1i("fApplyMainTexture", fApplyMainTexture ? 1 : 0);
			object->GetMaterial()->GetShader()->SetUniform1i("fApplyNormalTexture", fApplyNormalTexture ? 1 : 0);
			RefractiveIndex = 1.0f / Transmittance.ETA;
			object->GetMaterial()->GetShader()->SetUniform3f("refractiveIndexRGB", Transmittance.ETARGB.r, Transmittance.ETARGB.g, Transmittance.ETARGB.b);
			object->GetMaterial()->GetShader()->SetUniform1f("FresnelPower", Transmittance.FresnelPower);
			object->GetMaterial()->GetShader()->SetUniform1f("fBumpStrength", fBumpStrength);
			object->GetMaterial()->GetShader()->SetUniform1i("fFlipNormalInMatrix", fFlipNormalInMatrix ? 1 : 0);
			object->GetMaterial()->GetShader()->SetUniform1i("fFlipTangent", fFlipTangent ? 1 : 0);
			object->GetMaterial()->GetShader()->SetUniform1f("fTexMultiplier", fTexMultiplier);
			if (object == GlobalObjects[2].Teapot) {
				object->SetRotation(object->GetRotation() + glm::vec3(0.0f, 20.0f * gWindow->GetDeltaTime(), 0.0f));
			}
			//object->SetRotation(object->GetRotation() + glm::vec3(0.0f, 30.0f * gWindow->GetDeltaTime(), 0.0f));
			if (ShouldObjectsRotate) {
				object->SetRotation(object->GetRotation() + glm::vec3(0.0f, 45.0f * gWindow->GetDeltaTime(), 0.0f));
			}
		}
		CheckeredTexture->SetMinificationType(MinMagProperty.minType);
		CheckeredTexture->SetMagnificationType(MinMagProperty.magType);
		CheckeredTexture->SetLODBias(MinMagProperty.LODbias);

		TileTexture0->SetMinificationType(MinMagProperty.minType);
		TileTexture0->SetMagnificationType(MinMagProperty.magType);
		TileTexture0->SetLODBias(MinMagProperty.LODbias);

		ELightObject.LightObject->SetTranslation(LightObject->GetPosition());
		RenderImGui();
		ProcessInputs();
	}



	void InitializeImGui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImFontAtlas* atlas = io.Fonts;
		ImFontConfig* config = new ImFontConfig;
		config->GlyphRanges = atlas->GetGlyphRangesDefault();
		config->PixelSnapH = true;
		atlas->AddFontFromFileTTF("./External/Headers/imgui/misc/fonts/Cousine-Regular.ttf", 15, config);
		ImGuiStyle* style = &ImGui::GetStyle();
		ImGui::StyleColorsLight();
		ImGui_ImplGlfw_InitForOpenGL(gWindow->GetWindow(), true);
		ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);
	}
	void RenderImGui() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//EnableImGUIDockspace();
		ImGui::Begin("Game Debugging Window");
		if (ImGui::CollapsingHeader("Global Light")) {
			if (ImGui::TreeNode("Position")) {
				if (ImGui::BeginTable("PositionAttributes", 3))
				{
					ImGui::TableNextColumn();
					ImGui::InputFloat("X", &LightObject->Position.x);
					ImGui::TableNextColumn();
					ImGui::InputFloat("Y", &LightObject->Position.y);
					ImGui::TableNextColumn();
					ImGui::InputFloat("Z", &LightObject->Position.z);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Color")) {
				if (ImGui::BeginTable("ColorAttributes", 3))
				{
					ImGui::TableNextColumn();
					ImGui::InputFloat("R", &LightObject->Color.r);
					ImGui::TableNextColumn();
					ImGui::InputFloat("G", &LightObject->Color.g);
					ImGui::TableNextColumn();
					ImGui::InputFloat("B", &LightObject->Color.b);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Parameters")) {
				if (ImGui::BeginTable("LightAttributes", 1))
				{
					ImGui::TableNextColumn();
					ImGui::Text("Default Light Attributes");
					ImGui::InputFloat("Ambient", &LightObject->LightData.AmbientStrength);
					ImGui::InputFloat("Diffuse", &LightObject->LightData.DiffuseStrength);
					ImGui::InputFloat("Specular", &LightObject->LightData.SpecularStrength);
					ImGui::InputFloat("Specular Power", &LightObject->LightData.SpecularPower);
					ImGui::Text("Oren-Nayar Lighting Parameters");
					ImGui::InputFloat("roughness", &ONLightData.Roughness);
					ImGui::InputFloat("Irradiance", &ONLightData.Irradiance);
					ImGui::InputFloat("Albedo", &ONLightData.Albedo);
					ImGui::Text("Toon Lighting Parameters");
					ImGui::SliderInt("Intensity Multiplier", &fIntensityMultiplier, 1, 5);
					ImGui::Text("Cook-Torrance Lighting Parameters");
					ImGui::InputFloat("Fresnel", &CTLightData.F0);
					ImGui::InputFloat("Roughness", &CTLightData.Roughness);
					ImGui::InputFloat("K", &CTLightData.k);
					ImGui::Text("Minnaert Lighting Parameters");
					ImGui::InputFloat("Darkness factor", &MinLightData.k);

					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Transmissive & Reflectance parameters")) {
				if (ImGui::BeginTable("LightAttributes", 1))
				{
					ImGui::TableNextColumn();
					ImGui::Text("Default Light Attributes");
					//ImGui::InputFloat("ETA", &Transmittance.ETA);
					ImGui::InputFloat("EtaR", &Transmittance.ETARGB.r);
					ImGui::InputFloat("EtaG", &Transmittance.ETARGB.g);
					ImGui::InputFloat("EtaB", &Transmittance.ETARGB.b);

					ImGui::InputFloat("Fresnel power", &Transmittance.FresnelPower);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Object Material Attributes")) {
				if (ImGui::BeginTable("MaterialAttributes", 1))
				{
					ImGui::TableNextColumn();
					ImGui::Checkbox("fFlipBitangentCross", &fFlipBitangentCross);
					ImGui::Checkbox("fApplyMainTexture", &fApplyMainTexture);
					ImGui::Checkbox("fApplyNormalTexture", &fApplyNormalTexture);
					ImGui::Checkbox("Rotate Objects?", &ShouldObjectsRotate);
					ImGui::Checkbox("Apply Textures?", &ApplyTextures);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Skyboxes")) {
				if (ImGui::BeginTable("MaterialAttributes", 1))
				{
					ImGui::TableNextColumn();
					static ImGuiComboFlags flags = 0;


					std::string items[] = { "Default", "Skybox-1" };
					static int item_current_idx = 0;
					std::string combo_preview_value = items[item_current_idx];
					if (ImGui::BeginCombo("Skyboxes", combo_preview_value.c_str(), flags))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							const bool is_selected = (item_current_idx == n);
							if (ImGui::Selectable(items[n].c_str(), is_selected)) {
								item_current_idx = n;
								std::cout << CurrentSkybox << std::endl;
								std::cout << "Current Selected Index: " << items[item_current_idx] << std::endl;
								std::cout << SkyboxList[items[item_current_idx]] << std::endl;
								CurrentSkybox = SkyboxList[items[item_current_idx]];
								std::cout << CurrentSkybox << std::endl;
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Textures")) {
				if (ImGui::BeginTable("TextureAttribs", 1))
				{
					ImGui::TableNextColumn();
					static ImGuiComboFlags flags = 0;


					//std::string items[] = { "Default", "Skybox-1" };
					std::string items[] = { "Stone Wall", "Brick Wall - 0", "Brick Wall - 1", "Wood Texture" , "Gravel Texture" };
					//std::string items[] = {};
					static int item_current_idx = 0;
					std::string combo_preview_value = items[item_current_idx];
					if (ImGui::BeginCombo("Textures", combo_preview_value.c_str(), flags))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							const bool is_selected = (item_current_idx == n);
							if (ImGui::Selectable(items[n].c_str(), is_selected)) {
								item_current_idx = n;
								//MinMagProperty.minType = min_types[item]
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					static int min_flags = 0;
					//std::string items[] = { "Default", "Skybox-1" };
					std::string items_min_type[] = {
						"GL_NEAREST",
						"GL_LINEAR",
						"GL_NEAREST_MIPMAP_NEAREST",
						"GL_NEAREST_MIPMAP_LINEAR",
						"GL_LINEAR_MIPMAP_NEAREST",
						"GL_LINEAR_MIPMAP_LINEAR"
					};
					static int min_current_idx = 0;
					combo_preview_value = items_min_type[min_current_idx];
					if (ImGui::BeginCombo("Minification Type", combo_preview_value.c_str(), min_flags))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items_min_type); n++)
						{
							const bool is_selected = (min_current_idx == n);
							if (ImGui::Selectable(items_min_type[n].c_str(), is_selected)) {
								min_current_idx = n;
								MinMagProperty.minType = min_types[min_current_idx];
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//std::string items[] = { "Default", "Skybox-1" };
					std::string items_mag_type[] = {
						"GL_NEAREST",
						"GL_LINEAR"
					};
					static int mag_current_idx = 0;
					combo_preview_value = items_min_type[mag_current_idx];
					if (ImGui::BeginCombo("Magnification Type", combo_preview_value.c_str(), min_flags))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items_mag_type); n++)
						{
							const bool is_selected = (mag_current_idx == n);
							if (ImGui::Selectable(items_min_type[n].c_str(), is_selected)) {
								mag_current_idx = n;
								MinMagProperty.magType = mag_types[mag_current_idx];
							}

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::InputFloat("LOD-Bias", &MinMagProperty.LODbias);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::InputFloat("Texture Multiplier", &fTexMultiplier);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
		}


		if (ImGui::CollapsingHeader("Objects")) {
			for (Object* object : ResourceManager::gObjectList) {

				if (object->GetParent() == nullptr) {
					RenderObjectListTable(object);
				}
			}
		}
		if (ImGui::CollapsingHeader("Camera Settings")) {
			if (ImGui::BeginTable("CameraSettings", 1))
			{
				ImGui::TableNextColumn();
				if (ImGui::Button("Reset Camera")) {
					GlobalCamera->LookAt(CameraPositions[0][0], CameraPositions[0][1], glm::vec3(0.0f, 1.0f, 0.0f));
				}
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (ImGui::Button("Phong Model")) {
					GlobalCamera->LookAt(CameraPositions[1][0], CameraPositions[1][1], glm::vec3(0.0f, 1.0f, 0.0f));
				}
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (ImGui::Button("Toon Model")) {
					GlobalCamera->LookAt(CameraPositions[2][0], CameraPositions[2][1], glm::vec3(0.0f, 1.0f, 0.0f));
				}
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (ImGui::Button("Oren-Nayar Model")) {
					GlobalCamera->LookAt(CameraPositions[3][0], CameraPositions[3][1], glm::vec3(0.0f, 1.0f, 0.0f));
				}
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (ImGui::Button("Cook-Torrance")) {
					GlobalCamera->LookAt(CameraPositions[4][0], CameraPositions[4][1], glm::vec3(0.0f, 1.0f, 0.0f));
				}
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				if (ImGui::Button("Minnaert")) {
					GlobalCamera->LookAt(CameraPositions[5][0], CameraPositions[5][1], glm::vec3(0.0f, 1.0f, 0.0f));
				}
				ImGui::EndTable();
			}
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void RenderObjectListTable(Object* object) {
		if (ImGui::TreeNode(object->GetName().c_str())) {
			if (ImGui::TreeNode("Attributes")) {
				if (ImGui::BeginTable("ObjectAttributes", 3))
				{
					// ================================================================================================
					// ImGui - Position
					// ================================================================================================
					ImGui::TableNextRow();
					ImGui::Text("Position");
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::InputFloat("pX", &object->Translation.x);
					ImGui::TableNextColumn();
					ImGui::InputFloat("pY", &object->Translation.y);
					ImGui::TableNextColumn();
					ImGui::InputFloat("pZ", &object->Translation.z);
					// ================================================================================================
					// ImGui - Rotation
					// ================================================================================================
					ImGui::TableNextRow();
					ImGui::Text("Rotation");
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::InputFloat("rX", &object->Rotation.x);
					ImGui::TableNextColumn();
					ImGui::InputFloat("rY", &object->Rotation.y);
					ImGui::TableNextColumn();
					ImGui::InputFloat("rZ", &object->Rotation.z);
					// ================================================================================================
					// ImGui - Scale
					// ================================================================================================
					ImGui::TableNextRow();
					ImGui::Text("Scale");
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::InputFloat("sX", &object->Scale.x);
					ImGui::TableNextColumn();
					ImGui::InputFloat("sY", &object->Scale.y);
					ImGui::TableNextColumn();
					ImGui::InputFloat("sZ", &object->Scale.z);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}

			// ================================================================================================
			// ImGui - Show transform matrix (Read only)
			// ================================================================================================
			if (ImGui::TreeNode("Object Transform (Readonly)")) {
				if (ImGui::BeginTable("TransformAttributes", 4))
				{
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							ImGui::TableNextColumn();
							ImGui::InputFloat(("m" + std::to_string(i) + std::to_string(j)).c_str(), &object->mTransform[i][j]);
						}

						ImGui::TableNextRow();
					}
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			// ================================================================================================
			// ImGui - Child Objects
			// ================================================================================================
			if (object->GetChildObjects().size() > 0) {
				if (ImGui::TreeNode("Child Objects")) {
					for (Object* ChildObject : object->GetChildObjects()) {
						RenderObjectListTable(ChildObject);
					}
					ImGui::TreePop();
				}
			}

			// ================================================================================================
			// ImGui - Child Transforms
			// ================================================================================================
			if (object->GetChildTransforms().size() > 0) {
				if (ImGui::TreeNode("Child Transforms")) {
					for (Transform* transform : object->GetChildTransforms()) {
						if (ImGui::TreeNode(transform->GetName().c_str())) {
							if (ImGui::TreeNode("Transform Attributes")) {
								if (ImGui::BeginTable("PositionAttributes", 3))
								{
									// ================================================================================================
									// ImGui - Transform - Position
									// ================================================================================================
									ImGui::TableNextColumn();
									ImGui::Text("Position");
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::InputFloat("pX", &transform->Translation.x);
									ImGui::TableNextColumn();
									ImGui::InputFloat("pY", &transform->Translation.y);
									ImGui::TableNextColumn();
									ImGui::InputFloat("pZ", &transform->Translation.z);
									// ================================================================================================
									// ImGui - Transform - Rotation
									// ================================================================================================
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::Text("Rotation");
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::InputFloat("rX", &transform->Rotation.x);
									ImGui::TableNextColumn();
									ImGui::InputFloat("rY", &transform->Rotation.y);
									ImGui::TableNextColumn();
									ImGui::InputFloat("rZ", &transform->Rotation.z);
									// ================================================================================================
									// ImGui - Transform - Scale
									// ================================================================================================
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::Text("Scale");
									ImGui::TableNextRow();
									ImGui::TableNextColumn();
									ImGui::InputFloat("sX", &transform->Scale.x);
									ImGui::TableNextColumn();
									ImGui::InputFloat("sY", &transform->Scale.y);
									ImGui::TableNextColumn();
									ImGui::InputFloat("sZ", &transform->Scale.z);
									ImGui::EndTable();
								}
								ImGui::TreePop();
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	void EnableImGUIDockspace() {
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		bool p_open = true;
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			//ShowDockingDisabledMessage();
		}
		ImGui::End();

	}
	bool IsCursorHidden = true;

	void WindowMouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
		if (IsCursorHidden) {
			MouseData m_data;
			gWindow->GetMouseDelta(m_data, xpos, ypos);
			float mouseX = m_data.mouseY * 0.05f;
			float mouseY = -m_data.mouseX * 0.05f;
			GlobalCamera->RotateY(glm::radians(-m_data.mouseX * 0.05f));
			GlobalCamera->RotateX(glm::radians(m_data.mouseY * 0.05f));
		}
	}

	void WindowKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_REPEAT) {
			KeyMap.Set(key, true);
		}
		else if (action == GLFW_RELEASE) {
			KeyMap.Set(key, false);
		}

		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_ESCAPE:
			{

				if (!IsCursorHidden)
					gWindow->HideCursor();
				else
					gWindow->ShowCursor();
				IsCursorHidden = !IsCursorHidden;
				break;
			}
			}
		}


	}
	class Globals {
	public:
		static float gCameraMoveSpeed;
		static float gDeltaTime;
	};
	float Globals::gCameraMoveSpeed = 5.0f;
	float Globals::gDeltaTime = 1.0f;
	void ProcessInputs() {

		Globals::gDeltaTime = gWindow->GetDeltaTime();
		glm::vec3 MoveVector = glm::vec3(0.0f);
		if (KeyMap.Get(GLFW_KEY_UP) || KeyMap.Get(GLFW_KEY_W)) {
			//std::cout << "Moving forward" << std::endl;
			glm::vec3 NewPosition = (GlobalCamera->GetForward() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			GlobalCamera->Translate(GlobalCamera->GetTranslation() + NewPosition);
		}
		if (KeyMap.Get(GLFW_KEY_DOWN) || KeyMap.Get(GLFW_KEY_S)) {
			glm::vec3 NewPosition = -(GlobalCamera->GetForward() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			GlobalCamera->Translate(GlobalCamera->GetTranslation() + NewPosition);
		}
		if (KeyMap.Get(GLFW_KEY_LEFT) || KeyMap.Get(GLFW_KEY_A)) {
			glm::vec3 NewPosition = -(GlobalCamera->GetRight() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			GlobalCamera->Translate(GlobalCamera->GetTranslation() + NewPosition);
			//MoveVector += NewPosition;

		}
		if (KeyMap.Get(GLFW_KEY_RIGHT) || KeyMap.Get(GLFW_KEY_D)) {
			glm::vec3 NewPosition = +(GlobalCamera->GetRight() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			GlobalCamera->Translate(GlobalCamera->GetTranslation() + NewPosition);
		}

	}
};