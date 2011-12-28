#pragma once

class IRender {
public:
	virtual ~IRender() {
	}

	virtual void Render() = 0;

protected:
	IRender() {
	}
};

class IRenderObject : public IRender {
public:
	virtual ~IRenderObject() {
	}

	virtual void InitGraphics() = 0;
	virtual void ShutdownGraphics() = 0;

protected:
	IRenderObject() {
	}
};