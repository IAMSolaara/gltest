//wrapper function to handle compilation of vertex shader.
//done this way to "keep main code clean".
unsigned int compileVertexShader(const std::string path) {
	unsigned int vertexShader = -1;

	std::ifstream sourceFile(path);

	if (sourceFile.is_open()) {
		std::stringstream sourceStringStream;
		sourceStringStream << sourceFile.rdbuf();
		
		std::string sourceString = sourceStringStream.str();

		const char* source = sourceString.c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &source, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n%s\n", infoLog);
		}

	}

	return vertexShader;
}

//wrapper function to handle compilation of fragment shader.
//done this way to "keep main code clean".
unsigned int compileFragmentShader(const std::string path) {
	unsigned int fragmentShader = -1;

	std::ifstream sourceFile(path);

	if (sourceFile.is_open()) {
		std::stringstream sourceStringStream;
		sourceStringStream << sourceFile.rdbuf();

		std::string sourceString = sourceStringStream.str();

		const char* source = sourceString.c_str();

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &source, NULL);
		glCompileShader(fragmentShader);

		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n%s\n", infoLog);
		}
	}

	return fragmentShader;
}

//wrapper function to create our shader program
unsigned int createShaderProgram() {
	#if 0
	unsigned int vertexShader = compileVertexShader("./res/shaders/vertex/simple.vs");
	unsigned int fragmentShader = compileFragmentShader("./res/shaders/fragment/simple.fs");
	#else
	unsigned int vertexShader = compileVertexShader("../shaders/vertex/simple.vs");
	unsigned int fragmentShader = compileFragmentShader("../shaders/fragment/simple.fs");
	#endif
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success; char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "GL Program Link Error:\n%s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
