#pragma once
#pragma region PROTO

// Component 
#define PROTO_COMP_RENDERER L"Proto_Comp_Renderer"
#define PROTO_COMP_TRANSFORM L"Proto_Comp_Transform"
#define PROTO_COMP_RECTTRANSFORM L"Proto_Comp_RectTransform"
#define PROTO_COMP_OBBCOLLIDER L"Proto_Comp_ObbCollider"
#define PROTO_COMP_ABBCOLLIDER L"Proto_Comp_ABBCollider"
#define PROTO_COMP_SPHERECOLLIDER L"Proto_Comp_SphereCollider"
#define PROTO_COMP_REGION L"Proto_Comp_Region"
#define PROTO_NAVI_INTERIOR L"Proto_Navi_Interior"

/* Graphics Shader */
#define PROTO_RES_DEBUG2DTARGETSHADER L"Proto_Res_Debug2DTargetShader"
#define PROTO_RES_DEBUGGINGDEFAULTSHADER L"Proto_Res_DebuggingDefaultShader"
#define PROTO_RES_DEBUGGINGDEFAULTWHITESHADER L"Proto_Res_DebuggingDefaultWhiteShader"
#define PROTO_RES_DEBUGGINGWIREFRAMESHADER L"Proto_Res_DebuggingWireframeShader"
#define PROTO_RES_SHADOWSHADER L"Proto_Res_ShadowShader"
#define PROTO_RES_GRIDSHADER L"Proto_Res_GridShader"
#define PROTO_RES_RECTSHADER L"Proto_Res_RectShader"
#define PROTO_RES_FINALDEFFEREDSHADER L"Proto_Res_FinalDefferedShader"
#define PROTO_RES_BLENDDEFFEREDSHADER L"Proto_Res_BlendDefferedShader"
#define PROTO_RES_INSTANCERECTSHADER L"Proto_Res_InstanceRectShader"
#define PROTO_RES_DEFAULTOBJECTSHADER L"Proto_Res_DefaultObjectShader"
#define PROTO_RES_ANIMMODELSHADER L"Proto_Res_AnimModelShader"
#define PROTO_RES_NOCULL_ANIMMODELSHADER L"Proto_Res_NoCull_AnimModelShader"
#define PROTO_RES_MODELSHADER L"Proto_Res_ModelShader"
#define PROTO_RES_NORMALOBJECTSHADER L"Proto_Res_NormalObjectShader"
#define PROTO_RES_SKYBOXSHADER L"Proto_Res_SkyBoxShader"
#define PROTO_RES_3DNORMALCUBESHADER L"Proto_Res_3DNormalCubeShader"
#define PROTO_RES_LIGHTDIRECTIONSHADER L"Proto_Res_LightDirectionShader"
#define PROTO_RES_LIGHTPOINTSHADER L"Proto_Res_LightPointShader"
#define PROTO_RES_LIGHTSPOTSHADER L"Proto_Res_LightSpotShader"
#define PROTO_RES_PARTICLE2DSHADER L"Proto_Res_Particle2DShader"
#define PROTO_RES_PARTICLEFLARE2DSHADER L"Proto_Res_ParticleFlare2DShader"
#define PROTO_RES_WATERSHADER L"Proto_Res_WaterShader"
#define PROTO_RES_EQUIPMENTSHADER L"Proto_Res_EquipmentShader"
#define PROTO_RES_2DUISHADER L"Proto_Res_2DUIShader"
#define PROTO_RES_2DANIMATEPARTICLESHADER L"Proto_Res_2DAnimateParticleShader"
#define PROTO_RES_2DFIRESHADER L"Proto_Res_2DFireShader"
#define PROTO_RES_2DFOGSHADER L"Proto_Res_2DFogShader"
#define PROTO_RES_TERRAINTESSALATIONSHADER L"Proto_Res_TerrainTessalationShader"
#define PROTO_RES_ENVIRONMENTMAPPING L"Proto_Res_EnvironmentMappingShader"
#define PROTO_RES_MIRRORSHADER L"Proto_Res_MirrorShader"
#define PROTO_RES_SCREENRENDERONBJSHADER L"Proto_Res_ScreenRenderObjShader"
#define PROTO_RES_EQUIPMENTSHADER L"Proto_Res_EquipmentShader"

/* Compute Shader*/
#define PROTO_RES_COMPUTEANIMATIONSHADER L"Proto_Res_ConputeAnimatonShader"
#define PROTO_RES_COMPUTEPARTICLE2DSHADER L"Proto_Res_ComputeParticle2DShader"
#define PROTO_RES_COMPUTEBLOODEFFECT2DSHADER L"Proto_Res_ComputeBloodEffect2DShader"
#define PROTO_RES_COMPUTEROTATIONEFFECT2DSHADER L"Proto_Res_ComputeRotationEffect2DShader"
#define PROTO_RES_COMPUTEEMITPARTICLE2DSHADER L"Proto_Res_ComputeEmitParticle2DShader"

// Particle 
#define PROTO_RES_PARTICLESYSTEM L"Proto_Res_ParitlceSystem"

// VIBuffer 
#define PROTO_RES_VIBUFFERPOINT L"Proto_Res_VIBufferPoint"
#define PROTO_RES_VIBUFFERRECT L"Proto_Res_VIBufferRect"
#define PROTO_RES_VIBUFFERNORMALRECT L"Proto_Res_VIBufferNormalRect"
#define PROTO_RES_VIBUFFERTRIANGLE L"Proto_Res_VIBufferTriangle"
#define PROTO_RES_VIINSTANCEBUFFERRECT L"Proto_Res_VIInstanceBufferRect"
#define PROTO_RES_VIBUFFERSPHERE L"Proto_Res_VIBufferSphere"
#define PROTO_RES_VIBUFFERNORMALSPHERE L"Proto_Res_VIBufferNormalSphere"
#define PROTO_RES_VIBUFFERCUBE L"Proto__Res_VIBufferCube"
#define PROTO_RES_VIBUFFERCELL L"Proto__Res_VIBufferCell"
#define PROTO_RES_VIBUFFERNORMALCUBE L"Proto_Res_VIBufferNormalCube"
#define PROTO_RES_VIBUFFERPLANE L"Proto_Res_VIBufferPlane"
#define PROTO_RES_VIBUFFERNORMALPLANE L"Proto_Res_VIBufferNormalPlane"
#define PROTO_RES_VIBUFFERGRID L"Proto_Res_VIBufferGrid"
#define PROTO_RES_VIBUFFERNORMALGRID L"Proto_Res_VIBufferNormalGrid"
#define PROTO_RES_VIBUFFERSKYBOX L"Proto_Res_VIBufferSkyBox"

// Actor 
#define PROTO_ACTOR_DEFFEREDCAMERA L"Proto_Actor_DefferedCamera"
#define PROTO_ACTOR_SHADOWCAMERA L"Proto_Actor_ShadowCamera"
#define PROTO_RES_MIRRORCAMERA L"Proto_Actor_MirrorCamera"
#define PROTO_ACTOR_DEFAULTCUBE L"Proto_Actor_DefaultCube"
#define PROTO_ACTOR_DEFAULTSPHERE L"Proto_Actor_DefaultSphere"
#define PROTO_ACTOR_DEUBGGINGDEFAULTOBJECT L"Proto_Actor_DebuggingDefaultObject"
#define PROTO_ACTOR_DEUBGGINGDEFAULTCELL L"Proto_Actor_DebuggingDefaultCell"
#define PROTO_ACTOR_GRID L"Proto_Actor_Grid"
#define PROTO_ACTOR_TERRAIN L"Proto_Actor_Terrain"
#define PROTO_ACTOR_SKYBOX L"Proto_Actor_SkyBox"
#define PROTO_ACTOR_PARTICLE L"Proto_Actor_Particle"
#define PROTO_ACTOR_ANIMATEPARTICLE L"Proto_Actor_AnimateParticle"
#define PROTO_ACTOR_FIRE L"Proto_Actor_Fire"
#define PROTO_ACTOR_FOG L"Proto_Actor_Fog"
#define PROTO_ACTOR_MIRROR L"Proto_Actor_Mirror"
#define PROTO_ACTOR_SCREENRENDEROBJ L"Proto_Actor_ScreenRenderObj"
#define PROTO_ACTOR_EQUIPMENT L"Proto_Actor_Equipment"

// Texture

#define PROTO_RES_PARTICLETEXTUREGROUP L"Proto_Res_ParticleTextureGroup"
#define PROTO_RES_ANIMPARTICLETEXTUREGROUP L"Proto_Res_AnimParticleTextureGroup"
#define PROTO_RES_FIRECOLORTEXTUREGROUP L"Proto_Res_FireColorTextureGroup"
#define PROTO_RES_FIRENOISETEXTUREGROUP L"Proto_Res_FireNoiseTextureGroup"
#define PROTO_RES_FIREALPHATEXTUREGROUP L"Proto_Res_FireAlphaTextureGroup"
#define PROTO_RES_FOGTEXTUREGROUP L"Proto_Res_FogTextureGroup"


// material 

#define PROTO_RES_MATERIAL L"Proto_Res_Material"

#pragma endregion PROTO

#pragma region LAYER

#define LAYER_CAM L"LayerCamera"
#define LAYER_DEFAULT L"LayerDefault"
#define LAYER_SKYBOX L"LayerSkyBox"
#define LAYER_TERRAIN L"LayerTerrain"
#define LAYER_PARTICLE L"LayerParticle"
#define LAYER_MIRROR L"LayerMirror"
#define LAYER_SCREENRENDEROBJ L"LayerScreenRenderObj"
#define LAYER_DEBUG L"Layer_Debug"
#define LAYER_ITEM L"Layer_Item"

#pragma endregion LAYER

#pragma region TAG

#define COM_TRANFSORM L"ComTranform"
#define COM_RENDERER L"ComRenderer"
#define COM_RECTTRANSFORM L"ComRectTransform"
#define COM_OBBCOLLIDER L"ComOBBCollider"
#define COM_SPHRECOLLIDER L"ComSphereCollider"
#define COM_ABBCOLLIDER L"ComAbbCollider"

#define RES_NAVIGATION L"ResNavigation"
#define RES_SHADER L"ResShader"
#define RES_TEXGROUP L"ResTexGroup"
#define RES_VIBUFFER L"ResVIBuffer"
#define RES_MODEL L"ResModel"
#define RES_PARTICLESYSTEM L"ResParticleSystem"

#pragma endregion TAG
