cbuffer MatrixBuffer {
	matrix WorldViewProj;
};

/******** VERTEX SHADER ********/

struct VertexIn {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelIn {
    float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelIn _vshader(VertexIn input) {
    PixelIn o;

	o.position = mul(input.position, WorldViewProj);
    o.color = input.color;

	return o;
}

/******** PIXEL SHADER ********/

float4 _pshader(PixelIn input) : SV_TARGET {
    return input.color;
}