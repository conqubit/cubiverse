#define square(x) (x)*(x)

cbuffer MatrixBuffer {
	matrix WorldViewProj;
	float3 camPosition;
	float _padding1;
};

cbuffer PlanetData {
	float3 sunDirection2;

	float radius;
	float atmosphereHeight;
	float atmosphereRadius;
	float sunIntensity;
	float _padding2;
};

static const int b = 64;
cbuffer OpticalDepthTable {
	float depthTable[b * b];
}

static const float3 sunDirection = float3(1, 0, 0);

static const float3 RGB = {0.680, 0.550, 0.440};

static const float PI = atan(1) * 4;

static const float rayleighConstant = 0.05f;
static const float mieConstant = 0.0015f;

static const float3 rayleighConst =
{
	rayleighConstant / pow(RGB[0], 4),
	rayleighConstant / pow(RGB[1], 4),
	rayleighConstant / pow(RGB[2], 4)
};

static const float3 mieConst =
{
	mieConstant / pow(RGB[0], 0.84),
	mieConstant / pow(RGB[1], 0.84),
	mieConstant / pow(RGB[2], 0.84)
};

float density(float height) {
	return exp(-height/0.25);
}

float depth(float height, float cosA) {
	int x = (int)(height * b);
	int a = (int)((1 - cosA) / 2 * b);
	return depthTable[(x * b) + a];
}

float rayleighPhase(float cosA) {
	return 0.75 * (1 + cosA*cosA);
}

float miePhase(float cosA) {
	float g = -0.95;
	return 1.5 * (1 - g*g) / (2 + g*g) * (1 + cosA*cosA) / pow(1 + g*g - 2 * g * cosA, 1.5);
}

/******** VERTEX SHADER ********/

struct VertexIn {
	float4 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct PixelIn {
	float4 position      : SV_POSITION;
	float3 normal        : NORMAL0;
	float3 camToVertexDirection  : NORMAL1;
	float4 color         : COLOR0;
	float4 rayleighColor : COLOR1;
	float4 mieColor      : COLOR2;
	float2 texcoord      : TEXCOORD;
};

static float sun = 30;

PixelIn ComputeScattering(PixelIn data, float3 start, float len) {
	int iterations = 50;
	float delta = len / iterations;
	float3 deltaPos = data.camToVertexDirection * delta;

	data.rayleighColor = float4(0, 0, 0, 1);
	data.mieColor = float4(0, 0, 0, 1);

	for (int i = 1; i < iterations; i++) {
		float3 pos = start + i * deltaPos;
		float distanceToCenter = length(pos);
		pos /= distanceToCenter;
		float normalizedHeight = (distanceToCenter - radius) / atmosphereHeight;
		float camCosA = dot(pos, -data.camToVertexDirection);
		float sunCosA = dot(pos, -sunDirection);
		float maxCosA = -sqrt(square(distanceToCenter) - square(radius)) / distanceToCenter;
		float d = density(normalizedHeight);
		float totalInScatter = 4 * PI;
		/*if (sunCosA < maxCosA) {
			sunCosA -= maxCosA;
			totalInScatter *= depth(height, camCosA) + depth(height, sunCosA);
			data.rayleighColor.rgb += d * exp(-rayleighConst * totalInScatter) * delta / 10;
		} else {*/
			totalInScatter *= depth(normalizedHeight, camCosA) + depth(normalizedHeight, sunCosA);
			data.rayleighColor.rgb += d * exp(-rayleighConst * totalInScatter) * delta;
			data.mieColor.rgb += d * exp(-mieConst * totalInScatter) * delta;
		//}
	}

	data.rayleighColor.rgb *= rayleighConst * sunIntensity;
	data.mieColor.rgb *= mieConst * sunIntensity;

	return data;
}

PixelIn SkyVShader(VertexIn input) {
    PixelIn o;
	o.normal = input.normal;
	o.camToVertexDirection = input.position.xyz - camPosition;
	float camToVertexDistance = length(o.camToVertexDirection);
	o.camToVertexDirection /= camToVertexDistance;
	float camToHalfChordDistance = dot(-camPosition, o.camToVertexDirection);
	float le = length(camPosition);
	float deviation = acos(camToHalfChordDistance / le);
	float startDist = 2*camToHalfChordDistance - camToVertexDistance;
	float3 pos2 = camPosition + o.camToVertexDirection * startDist;
	float end = camToVertexDistance - startDist;

	if (deviation >= asin(radius / le) - 0.05) {
		o = ComputeScattering(o, pos2, end);
	}
	
	o.color = input.color;
	o.position = mul(input.position, WorldViewProj);
	o.texcoord = input.texcoord;

	return o;
}

PixelIn GroundVShader(VertexIn input) {
    PixelIn output;
	output.normal = input.normal;
	output.texcoord = input.texcoord;
	output.camToVertexDirection = input.position.xyz - camPosition;

	float camToVertexDistance = length(output.camToVertexDirection);
	output.camToVertexDirection /= camToVertexDistance;
	float camToHalfChordDistance = dot(-camPosition, output.camToVertexDirection);

	float centerToHalfChordDistanceSquared = dot(camPosition, camPosition) - square(camToHalfChordDistance);
	float atmosphereIntersectionToHalfChordDistance = sqrt(square(atmosphereRadius) - centerToHalfChordDistanceSquared);
	float groundIntersectionToHalfChordDistance = sqrt(abs(square(radius) - centerToHalfChordDistanceSquared));
	float camToAtmosphereIntersectionDistance = camToHalfChordDistance - atmosphereIntersectionToHalfChordDistance;
	
	float3 startPosition = camPosition + output.camToVertexDirection * camToAtmosphereIntersectionDistance;
	float atmosphereDepthLength = atmosphereIntersectionToHalfChordDistance - groundIntersectionToHalfChordDistance;

	output = ComputeScattering(output, startPosition, atmosphereDepthLength);

	output.color = float4(0.1, 0.1, 0.1, 1);
	/*float sunCosA = dot(input.normal, -sunDirection);
	if (sunCosA > 0) {
		o.color.rgb += exp(-rayleighConst * depth(sunCosA, 0))*sunCosA;
		o.color.a = 1;
	}*/
	output.position = mul(input.position, WorldViewProj);

	return output;
}

/******** PIXEL SHADER ********/

Texture2D shaderTexture;
SamplerState SampleType;

float4 Ground(PixelIn input) : SV_TARGET {
	float cosA = dot(sunDirection, -input.camToVertexDirection);
	input.rayleighColor.rgb *= rayleighPhase(cosA);
	input.mieColor.rgb *= miePhase(cosA);
	float camCosA = dot(input.normal, -camPosition)/length(camPosition);
    return (shaderTexture.Sample(SampleType, input.texcoord)) + input.rayleighColor;// + input.mieColor;
}

float4 Sky(PixelIn input) : SV_TARGET {
	float cosA = dot(sunDirection, -input.camToVertexDirection);
	input.rayleighColor.rgb *= rayleighPhase(cosA);
	input.mieColor.rgb *= miePhase(cosA);
	return input.rayleighColor + input.mieColor;
}