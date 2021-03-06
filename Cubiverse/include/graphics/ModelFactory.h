#pragma once

class Model;
class Shader;

class ModelFactory {
public:
	struct Attribute {
		string name;
		int count;
		int offset;
		GLenum glType;
		GLuint shaderLoc;
		bool normalized;
		bool hidden;
	};

	std::vector<Attribute> attributes;

private:
	std::vector<byte> vertexData;
	std::vector<int> indices;

	int stride;
	int pos;

	template <typename I>
	byte* GetAttributePtr(const I& ident) {
		return &vertexData[pos + GetAttribute(ident).offset];
	}

public:
	Shader* shader;
	Texture* texture;

	byte* dataOverride;
	int sizeOverride;

	GLenum usage;

	GLenum topology;

	ModelFactory() :
	stride(0),
	pos(0),

	shader(nullptr),
	texture(nullptr),
	dataOverride(nullptr),
	sizeOverride(-1),
	usage(GL_STATIC_DRAW),
	topology(GL_TRIANGLES) {
	}

	~ModelFactory() {
	}

	template <typename T>
	GLenum GetGLType() { return 0; }

	template <> GLenum GetGLType<char>() { return GL_BYTE; }
	template <> GLenum GetGLType<byte>() { return GL_UNSIGNED_BYTE; }
	template <> GLenum GetGLType<short>() { return GL_SHORT; }
	template <> GLenum GetGLType<ushort>() { return GL_UNSIGNED_SHORT; }
	template <> GLenum GetGLType<int>() { return GL_INT; }
	template <> GLenum GetGLType<uint>() { return GL_UNSIGNED_INT; }
	template <> GLenum GetGLType<float>() { return GL_FLOAT; }
	template <> GLenum GetGLType<double>() { return GL_DOUBLE; }

	template <typename T>
	int AddAttribute(const string& name, int count, bool normalized = false, bool hidden = false) {
		int index = (int)attributes.size();

		Attribute a;
		a.name = name;
		a.count = count;
		a.offset = stride;
		a.glType = GetGLType<T>();
		a.normalized = normalized;
		a.hidden = hidden;
		attributes.push_back(a);

		stride += sizeof(T) * count;
		pos = -stride;
		return index;
	}

	const Attribute& GetAttribute(int index)const {
		return attributes[index];
	}

	const Attribute& GetAttribute(const string& name)const {
		for (int i = 0; i < attributes.size(); i++) {
			if (name == attributes[i].name) {
				return attributes[i];
			}
		}
	}

	int AttributeCount()const {
		return (int)attributes.size();
	}

	ModelFactory& Next() {
		pos += stride;
		vertexData.resize(pos + stride);
		return *this;
	}

	template <typename I, typename T>
	ModelFactory& Set(const I& ident, T x) {
		float* data = (float*)GetAttributePtr(ident);
		data[0] = (float)x;
		return *this;
	}

	template <typename I, typename T, typename U>
	ModelFactory& Set(const I& ident, T x, U y) {
		float* data = (float*)GetAttributePtr(ident);
		data[0] = (float)x;
		data[1] = (float)y;
		return *this;
	}

	template <typename I, typename T, typename U, typename V>
	ModelFactory& Set(const I& ident, T x, U y, V z) {
		float* data = (float*)GetAttributePtr(ident);
		data[0] = (float)x;
		data[1] = (float)y;
		data[2] = (float)z;
		return *this;
	}

	template <typename I, typename T, typename U, typename V, typename W>
	ModelFactory& Set(const I& ident, T x, U y, V z, W w) {
		float* data = (float*)GetAttributePtr(ident);
		data[0] = (float)x;
		data[1] = (float)y;
		data[2] = (float)z;
		data[3] = (float)w;
		return *this;
	}

	template <typename I, typename T>
	ModelFactory& Set(const I& ident, const Vector3<T>& v) {
		float* data = (float*)GetAttributePtr(ident);
		data[0] = (float)v.x;
		data[1] = (float)v.y;
		data[2] = (float)v.z;
		return *this;
	}

	template <typename I, typename T>
	ModelFactory& Set(const I& ident, const Color<T>& c) {
		float* data = (float*)GetAttributePtr(ident);
		data[0] = (float)c.r;
		data[1] = (float)c.g;
		data[2] = (float)c.b;
		data[3] = (float)c.a;
		return *this;
	}

	template <typename I>
	ModelFactory& Set(const I& ident, const ColorB& c) {
		byte* data = GetAttributePtr(ident);
		data[0] = c.r;
		data[1] = c.g;
		data[2] = c.b;
		data[3] = c.a;
		return *this;
	}

	template <typename I, typename T>
	ModelFactory& SetShort(const I& ident, T x) {
		short* data = (short*)GetAttributePtr(ident);
		data[0] = (short)x;
	}

	template <typename I, typename T>
	ModelFactory& SetUShort(const I& ident, T x) {
		ushort* data = (ushort*)GetAttributePtr(ident);
		data[0] = (ushort)x;
	}

	void AddTriangle(int a, int b, int c) {
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}

	void AddSegment(int a, int b) {
		indices.push_back(a);
		indices.push_back(b);
	}

	int VertexCount()const {
		return VertexDataSize() / stride;
	}

	int VertexStride()const {
		return stride;
	}

	int VertexDataSize()const {
		if (sizeOverride != -1) {
			return sizeOverride;
		}
		return (int)vertexData.size();
	}

	int IndexCount()const {
		return (int)indices.size();
	}

	byte* VertexData()const {
		if (dataOverride) {
			return dataOverride;
		}
		return (byte*)vertexData.data();
	}

	int* IndexData()const {
		return (int*)indices.data();
	}

	Model* Create(int buffExtra = 0);

	void Clear() {
		vertexData.clear();
		//indices.clear();
		pos = -stride;
		dataOverride = nullptr;
		sizeOverride = -1;
	}

	static Model* CreateWireframeDebugBox(const BoundingBoxD& b, const ColorF& color) {
		ModelFactory mf;
		mf.shader = Res::GetShader("color");
		mf.topology = GL_LINES;
		mf.AddAttribute<float>("position", 3);
		mf.AddAttribute<float>("color", 4);

		mf.Next().Set("position", b.min).Set("color", color);
		mf.Next().Set("position", b.max.x, b.min.y, b.min.z).Set("color", color);

		mf.Next().Set("position", b.max.x, b.min.y, b.min.z).Set("color", color);
		mf.Next().Set("position", b.max.x, b.max.y, b.min.z).Set("color", color);

		mf.Next().Set("position", b.max.x, b.max.y, b.min.z).Set("color", color);
		mf.Next().Set("position", b.min.x, b.max.y, b.min.z).Set("color", color);

		mf.Next().Set("position", b.min.x, b.max.y, b.min.z).Set("color", color);
		mf.Next().Set("position", b.min).Set("color", color);


		mf.Next().Set("position", b.min.x, b.min.y, b.max.z).Set("color", color);
		mf.Next().Set("position", b.max.x, b.min.y, b.max.z).Set("color", color);

		mf.Next().Set("position", b.max.x, b.min.y, b.max.z).Set("color", color);
		mf.Next().Set("position", b.max.x, b.max.y, b.max.z).Set("color", color);

		mf.Next().Set("position", b.max.x, b.max.y, b.max.z).Set("color", color);
		mf.Next().Set("position", b.min.x, b.max.y, b.max.z).Set("color", color);

		mf.Next().Set("position", b.min.x, b.max.y, b.max.z).Set("color", color);
		mf.Next().Set("position", b.min.x, b.min.y, b.max.z).Set("color", color);


		mf.Next().Set("position", b.min).Set("color", color);
		mf.Next().Set("position", b.min.x, b.min.y, b.max.z).Set("color", color);

		mf.Next().Set("position", b.max.x, b.min.y, b.min.z).Set("color", color);
		mf.Next().Set("position", b.max.x, b.min.y, b.max.z).Set("color", color);

		mf.Next().Set("position", b.max.x, b.max.y, b.min.z).Set("color", color);
		mf.Next().Set("position", b.max.x, b.max.y, b.max.z).Set("color", color);

		mf.Next().Set("position", b.min.x, b.max.y, b.min.z).Set("color", color);
		mf.Next().Set("position", b.min.x, b.max.y, b.max.z).Set("color", color);
		
		return mf.Create();
	}
};