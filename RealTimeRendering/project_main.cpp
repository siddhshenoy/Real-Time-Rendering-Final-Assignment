#include <iostream>

#include <Core/Window.h>

#include <Rendering/Light.h>
#include <Resource/ResourceManager.h>
#include <Rendering/FrameBuffer.h>
#include <Rendering/Sprite.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#define			IMGUI_GLSL_VERSION			"#version 130"


#define			VERTEX_SHADER_IDX			0
#define			FRAGMENT_SHADER_IDX			1

void InitializeImGui();
void RenderImGui();
void RenderObjectListTable(Object* object);
void CherryTheme();
void BenchmarkMemory(const std::string& pretext = "");
typedef struct SSS_PARAMS {
	bool UseHalfVector = true;
	float fCosThetaPower = 5.0f;
	float fFresnelPower = 1.0f;
	float fScale = 200.0f;
	float fBias = 200.0f;
}
SubSurfaceScatteringParams;

int numPasses = 250;
bool useBlurredImage = false;

namespace EngineVariables {
	//========================================
	// WINDOW
	//========================================
	static Window* gWindow;
	//========================================
	// LIGHT
	//========================================
	static Light* gLight;
	//========================================
	// TEXTURE
	//========================================
	static Texture* gSkinTexture;
	static Texture* gBeckmannTexture;
	static Texture* gSpecularTexture;
	static Texture* gNormalTexture;
	//========================================
	// SHADER
	//========================================
	static BaseShader* gSubSurfaceBaseShaders[2];
	static Shader* gSubSurfaceShader;

	//========================================
	// SUBSURFACE SCATTERING PARAMETERS
	//========================================
	static SubSurfaceScatteringParams gSubSurfaceParams;
	static bool UseBeckmannTexture;
	static bool UseNormalMap = true;
	static bool UseTexture = true;
	static bool UseOnlySpecular = false;
	
	//========================================
	// MATERIALS
	//========================================
	static Material* gSubSurfaceMaterial;
	//========================================
	// OBJECTS
	//========================================
	static Object* gHumanFace;
	//========================================
	// CAMERA
	//========================================
	static Camera* gCamera;
	//========================================
	// FrameBuffers
	//========================================
	static BaseShader* FrameBufferBaseShader[2];
	static Shader* FrameBufferShader;

	static BaseShader* YBlurBaseShaders[2];
	static Shader* YBlurShader;
	static BaseShader* XBlurBaseShaders[2];
	static Shader* XBlurShader;

	static Sprite* BlurBufferSprite;
	static Sprite* BlurBufferSpriteY0;
	static FrameBuffer* BlurBufferY0;
	static FrameBuffer* BlueBufferX0;

	static Texture* BlurTextures[10];
}
namespace EngineFunctions {
	class WindowFunctions {
		public:
			static void InitFunction(GLFWwindow* window);
			static void RenderFunction(GLFWwindow* window);
	};
};

void EngineFunctions::WindowFunctions::InitFunction(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//
	// CAMERA SETUP
	//
	EngineVariables::gCamera = new Camera();
	EngineVariables::gCamera->SetProjectionType(ProjectionType::PERSPECTIVE);
	EngineVariables::gCamera->Translate(glm::vec3(0.0f, 0.0f, 10.0f));
	EngineVariables::gCamera->LookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//======================================================================
	// LIGHT SETUP
	//======================================================================
	EngineVariables::gLight = new Light();
	EngineVariables::gLight->SetPosition(glm::vec3(0.0f, 10.0f, -5.0f));
	EngineVariables::gLight->SetColor(glm::vec3(1.0f));

	//======================================================================
	// SKYBOX SETUP - IF AT ALL NEEDED
	//======================================================================
	//======================================================================
	// TEXTURE SETUP
	//======================================================================
	BenchmarkMemory("Before Skin texture");
	EngineVariables::gSkinTexture = new Texture();
	//EngineVariables::gSkinTexture->LoadTexture("./GameResources/Textures/Skin_Human_002_SD/Skin_Human_002_COLOR.png");
	EngineVariables::gSkinTexture->LoadTexture("./GameResources/Textures/texture1_500x500.png");
	BenchmarkMemory("After Skin texture");

	BenchmarkMemory("Before Beckmann texture");
	EngineVariables::gBeckmannTexture = new Texture();
	EngineVariables::gBeckmannTexture->LoadTexture("./GameResources/Textures/beckmanntex.jpg");
	BenchmarkMemory("After Beckmann texture");
	
	BenchmarkMemory("Before specular texture");
	EngineVariables::gSpecularTexture = new Texture();
	EngineVariables::gSpecularTexture->LoadTexture("./GameResources/Textures/texture1_SPEC_500x500.png");
	BenchmarkMemory("After specular texture");

	BenchmarkMemory("Before normal map texture");
	EngineVariables::gNormalTexture = new Texture();
	EngineVariables::gNormalTexture->LoadTexture("./GameResources/Textures/texture_normalmap.png");
	BenchmarkMemory("After normal map texture");
	//======================================================================
	// SHADER SETUP
	//======================================================================
	EngineVariables::gSubSurfaceBaseShaders[VERTEX_SHADER_IDX] = new BaseShader();
	EngineVariables::gSubSurfaceBaseShaders[FRAGMENT_SHADER_IDX] = new BaseShader();
	EngineVariables::gSubSurfaceBaseShaders[VERTEX_SHADER_IDX]->CreateBasicShader("SubSurface Vertex Shader", "./GameResources/Shaders/FinalProject/SSS_VS.glsl", GL_VERTEX_SHADER);
	EngineVariables::gSubSurfaceBaseShaders[FRAGMENT_SHADER_IDX]->CreateBasicShader("SubSurface Fragment Shader", "./GameResources/Shaders/FinalProject/SSS_FS.glsl", GL_FRAGMENT_SHADER);

	EngineVariables::gSubSurfaceShader = new Shader(std::vector<BaseShader*>({
			EngineVariables::gSubSurfaceBaseShaders[VERTEX_SHADER_IDX],
			EngineVariables::gSubSurfaceBaseShaders[FRAGMENT_SHADER_IDX]
		})
	);


	EngineVariables::YBlurBaseShaders[VERTEX_SHADER_IDX] = new BaseShader();
	EngineVariables::YBlurBaseShaders[FRAGMENT_SHADER_IDX] = new BaseShader();
	EngineVariables::YBlurBaseShaders[VERTEX_SHADER_IDX]->CreateBasicShader("YBlur Base Vertex Shader", "./GameResources/Shaders/FinalProject/YBlurVertexShader.glsl", GL_VERTEX_SHADER);
	EngineVariables::YBlurBaseShaders[FRAGMENT_SHADER_IDX]->CreateBasicShader("YBlur Base Fragment Shader", "./GameResources/Shaders/FinalProject/YBlurFragmentShader.glsl", GL_FRAGMENT_SHADER);

	EngineVariables::YBlurShader = new Shader(std::vector<BaseShader*>({
			EngineVariables::YBlurBaseShaders[VERTEX_SHADER_IDX],
			EngineVariables::YBlurBaseShaders[FRAGMENT_SHADER_IDX]
		})
	);
	EngineVariables::FrameBufferBaseShader[VERTEX_SHADER_IDX] = new BaseShader();
	EngineVariables::FrameBufferBaseShader[FRAGMENT_SHADER_IDX] = new BaseShader();
	EngineVariables::FrameBufferBaseShader[VERTEX_SHADER_IDX]->CreateBasicShader("Framebuffer Base Vertex Shader", "./GameResources/Shaders/FinalProject/FramebufferVertexShader.glsl", GL_VERTEX_SHADER);
	EngineVariables::FrameBufferBaseShader[FRAGMENT_SHADER_IDX]->CreateBasicShader("Framebuffer Base Fragment Shader", "./GameResources/Shaders/FinalProject/FramebufferFragmentShader.glsl", GL_FRAGMENT_SHADER);

	EngineVariables::FrameBufferShader = new Shader(std::vector<BaseShader*>({
			EngineVariables::FrameBufferBaseShader[VERTEX_SHADER_IDX],
			EngineVariables::FrameBufferBaseShader[FRAGMENT_SHADER_IDX]
		})
	);
	//======================================================================
	// MATERIAL SETUP
	//======================================================================
	EngineVariables::gSubSurfaceMaterial = new Material();
	EngineVariables::gSubSurfaceMaterial->AttachShader(EngineVariables::gSubSurfaceShader);
	EngineVariables::gSubSurfaceMaterial->AttachTexture("Skin Texture", EngineVariables::gSkinTexture, 0);
	EngineVariables::gSubSurfaceMaterial->AttachTexture("Beckmann Texture", EngineVariables::gBeckmannTexture, 1);
	EngineVariables::gSubSurfaceMaterial->AttachTexture("Specular Texture", EngineVariables::gSpecularTexture, 2);
	EngineVariables::gSubSurfaceMaterial->AttachTexture("Normal Texture", EngineVariables::gNormalTexture, 3);
	EngineVariables::gSubSurfaceMaterial->AttachTexture("Blur Texture", EngineVariables::gSkinTexture, 4);
	//======================================================================
	// OBJECT SETUP
	//======================================================================
	//EngineVariables::gHumanFace = ResourceManager::LoadObject("./GameResources/Models/FemaleHead/FemaleHead.fbx");
	//EngineVariables::gHumanFace = ResourceManager::LoadObject("./GameResources/Models/MaleHead/shape1_original.ply");
	BenchmarkMemory("Before loading head model");
	EngineVariables::gHumanFace = ResourceManager::LoadObject("./GameResources/Models/MaleHead/shape1_refined.fbx");
	BenchmarkMemory("After loading head model");
	EngineVariables::gHumanFace->AssignMaterial(EngineVariables::gSubSurfaceMaterial);
	
	EngineVariables::gHumanFace->SetTranslation(glm::vec3(0.0f, 0.0f, 5.0f));
	EngineVariables::gHumanFace->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
	EngineVariables::gHumanFace->SetScale(glm::vec3(0.01f));

	InitializeImGui();
	
}
GLuint old_texture = 0;
void EngineFunctions::WindowFunctions::RenderFunction(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	
	int windowWidth, windowHeight;
	EngineVariables::gWindow->GetDimensions(&windowWidth, &windowHeight);
	if (windowWidth > 0 && windowHeight > 0)
		EngineVariables::gCamera->InitializePerspectiveParameters(70.0f, ((float)windowWidth / (float)windowHeight), 0.001f, 1000.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, windowWidth, windowHeight);
	
	for (Object* object : ResourceManager::gObjectList) {
		object->Render(EngineVariables::gCamera);

		object->GetMaterial()->GetShader()->SetUniform1i("Texture", 0);
		object->GetMaterial()->GetShader()->SetUniform1i("gBeckmannTexture", 1);
		object->GetMaterial()->GetShader()->SetUniform1i("gSpecularMap", 2);
		object->GetMaterial()->GetShader()->SetUniform1i("NormalMap", 3);

		object->GetMaterial()->GetShader()->SetUniform1i("useBeckmannTexture", EngineVariables::UseBeckmannTexture ? 1 : 0);
		object->GetMaterial()->GetShader()->SetUniform1i("useNormalMap", EngineVariables::UseNormalMap ? 1 : 0);
		object->GetMaterial()->GetShader()->SetUniform1i("useTexture", EngineVariables::UseTexture ? 1 : 0);
		object->GetMaterial()->GetShader()->SetUniform1i("useOnlySpecular", EngineVariables::UseOnlySpecular ? 1 : 0);

		object->GetMaterial()->GetShader()->SetUniform1f("fFresnelFactor", EngineVariables::gSubSurfaceParams.fFresnelPower);
		object->GetMaterial()->GetShader()->SetUniform1f("cosThetaPower", EngineVariables::gSubSurfaceParams.fCosThetaPower);
		
		object->GetMaterial()->GetShader()->SetUniform1f("bias", EngineVariables::gSubSurfaceParams.fBias);
		object->GetMaterial()->GetShader()->SetUniform1f("scale", EngineVariables::gSubSurfaceParams.fScale);
		object->GetMaterial()->GetShader()->SetUniform3f("fLightColor", EngineVariables::gLight->GetColor());
		object->GetMaterial()->GetShader()->SetUniform3f("fLightPosition", EngineVariables::gLight->GetPosition());
		object->GetMaterial()->GetShader()->SetUniform3f("fCameraPosition", EngineVariables::gCamera->GetTranslation());
		object->GetMaterial()->GetShader()->SetUniform1f("fAmbientStrength", EngineVariables::gLight->LightData.AmbientStrength);
		object->GetMaterial()->GetShader()->SetUniform1f("fDiffuseStrength", EngineVariables::gLight->LightData.DiffuseStrength);
		object->GetMaterial()->GetShader()->SetUniform1f("fSpecularStrength", EngineVariables::gLight->LightData.SpecularStrength);
		object->GetMaterial()->GetShader()->SetUniform1f("fSpecularPower", EngineVariables::gLight->LightData.SpecularPower);

	}
	
	RenderImGui();
}
int main() {
	
	EngineVariables::gWindow = new Window();
	EngineVariables::gWindow->Create("Real Time Rendering - Final Project", 50, 50, 800, 600, false);
	GLint totalMemoryKb = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKb);
	GLint currentMemoryKb = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentMemoryKb);
	spdlog::info("Total Memory: {}", totalMemoryKb);
	spdlog::info("Current Memory: {}", currentMemoryKb);
	EngineVariables::gWindow->SetInitFunction(EngineFunctions::WindowFunctions::InitFunction);


	EngineVariables::gWindow->SetRenderingFunction(EngineFunctions::WindowFunctions::RenderFunction);
	EngineVariables::gWindow->ShowWindow();
	EngineVariables::gWindow->RenderLoop();
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
	ImGui_ImplGlfw_InitForOpenGL(EngineVariables::gWindow->GetWindow(), true);
	ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);
	CherryTheme();
}
void RenderImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//EnableImGUIDockspace();
	ImGui::Begin("Inspector");
	if (ImGui::CollapsingHeader("Global Light")) {
		if (ImGui::TreeNode("Position")) {
			if (ImGui::BeginTable("PositionAttributes", 3))
			{
				ImGui::TableNextColumn();
				ImGui::InputFloat("X", &EngineVariables::gLight->Position.x);
				ImGui::TableNextColumn();
				ImGui::InputFloat("Y", &EngineVariables::gLight->Position.y);
				ImGui::TableNextColumn();
				ImGui::InputFloat("Z", &EngineVariables::gLight->Position.z);
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Color")) {
			if (ImGui::BeginTable("ColorAttributes", 3))
			{
				ImGui::TableNextColumn();
				ImGui::InputFloat("R", &EngineVariables::gLight->Color.r);
				ImGui::TableNextColumn();
				ImGui::InputFloat("G", &EngineVariables::gLight->Color.g);
				ImGui::TableNextColumn();
				ImGui::InputFloat("B", &EngineVariables::gLight->Color.b);
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Parameters")) {
			if (ImGui::BeginTable("LightAttributes", 1))
			{
				ImGui::TableNextColumn();
				ImGui::Text("Default Light Attributes");
				ImGui::InputFloat("Ambient", &EngineVariables::gLight->LightData.AmbientStrength);
				ImGui::InputFloat("Diffuse", &EngineVariables::gLight->LightData.DiffuseStrength);
				ImGui::InputFloat("Specular", &EngineVariables::gLight->LightData.SpecularStrength);
				ImGui::InputFloat("Specular Power", &EngineVariables::gLight->LightData.SpecularPower);
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
	if (ImGui::CollapsingHeader("Subsurface Scattering Parameters")) {
		if (ImGui::BeginTable("LightAttributes", 2))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Generic Params");
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Checkbox("EngineVariables::UseOnlySpecular", &EngineVariables::UseOnlySpecular);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Checkbox("EngineVariables::UseBeckmannTexture", &EngineVariables::UseBeckmannTexture);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Checkbox("EngineVariables::UseNormalMap", &EngineVariables::UseNormalMap);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Checkbox("EngineVariables::UseTexture", &EngineVariables::UseTexture);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Specular Information");
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::InputFloat("Fresnel Power", &EngineVariables::gSubSurfaceParams.fFresnelPower);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::InputFloat("Cos Theta Power", &EngineVariables::gSubSurfaceParams.fCosThetaPower);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Checkbox("Use Half Vector", &EngineVariables::gSubSurfaceParams.UseHalfVector);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::InputFloat("SubSurface Bias", &EngineVariables::gSubSurfaceParams.fBias);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::InputFloat("Subsurface Scale", &EngineVariables::gSubSurfaceParams.fScale);
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

void CherryTheme() {
	// cherry colors, 3 intensities
#define HI(v)   ImVec4(0.35f, 0.35f, 0.35f, v)
#define MED(v)  ImVec4(0.20, 0.20, 0.20, v)
#define LOW(v)  ImVec4(0.25, 0.25, 0.25, v)
	// backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG(v)   ImVec4(0.15, 0.15f, 0.15f, v)
	// text
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = TEXT(0.78f);
	style.Colors[ImGuiCol_TextDisabled] = TEXT(0.28f);
	style.Colors[ImGuiCol_WindowBg] = BG(0.9f);//ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = BG(0.58f);
	style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = HI(0.25f); // slider background
	style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
	style.Colors[ImGuiCol_TitleBg] = LOW(1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = MED(0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style.Colors[ImGuiCol_ButtonActive] = MED(1.00f);
	style.Colors[ImGuiCol_Header] = MED(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style.Colors[ImGuiCol_HeaderActive] = HI(1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
	style.Colors[ImGuiCol_PlotLines] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = TEXT(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);

	style.WindowPadding = ImVec2(6, 4);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(5, 2);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(7, 1);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;

	style.WindowTitleAlign.x = 0.50f;

	style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
	style.FrameBorderSize = 0.0f;
	style.WindowBorderSize = 1.0f;
}
void BenchmarkMemory(const std::string& pretext) {
	if (pretext != "") std::cout << "[" << pretext << "]" << std::endl;
	int totalMemoryKb = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKb);
	int currentMemoryKb = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentMemoryKb);

	spdlog::info("Total Memory: {}", totalMemoryKb);
	spdlog::info("Current Memory: {}", currentMemoryKb);
}