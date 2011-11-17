cbuffer MatrixBuffer {
	matrix WorldViewProj;
};

/******** VERTEX SHADER ********/

struct VertexIn {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelIn {
    float4 position : POSITION;
	float4 color : COLOR;
};

PixelIn _vshader(VertexIn input) {
    PixelIn o;

	o.color = input.color;
	o.position = mul(input.position, WorldViewProj);

	return o;
}

/******** PIXEL SHADER ********/

float4 _pshader(PixelIn input) : SV_TARGET {
    return input.color;
}