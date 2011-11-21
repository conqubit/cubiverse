cbuffer MatrixBuffer {
	matrix WorldViewProj;
	float3 camPosition;
	float _padding1;
};

/******** VERTEX SHADER ********/

struct VertexIn {
	float4 position : POSITION;
	float3 color : COLOR;
};

struct PixelIn {
    float4 position : POSITION;
	float4 color : COLOR;
};

PixelIn _vshader(VertexIn input) {
    PixelIn o;

	o.color = input.color;
    o.color[3] = 1;
	o.position = mul(input.position, WorldViewProj);

	return o;
}

/******** PIXEL SHADER ********/

float4 _pshader(PixelIn input) : SV_TARGET {
    return input.color;
}