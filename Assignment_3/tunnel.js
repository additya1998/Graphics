function initBuffersTunnel(gl) {

	// Create a buffer for the cube's vertex positions.
	const positionBuffer = gl.createBuffer();

	// Select the positionBuffer as the one to apply buffer
	// operations to from here out.
	gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

	// Now create an array of positions for the cube.
	var sz = 5, angle = 22.5, z_f = 0.0, z_s = 5.0;
	const positions = [
		-sz * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		 sz * Math.sin(angle * Math.PI / 180.0),  sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		 sz * Math.sin(angle * Math.PI / 180.0),  sz * Math.cos(angle * Math.PI / 180.0),  -z_s,
		-sz * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0),  -z_s,

		-sz * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		-sz * Math.cos(angle * Math.PI / 180.0),  sz * Math.sin(angle * Math.PI / 180.0),  z_f,
		-sz * Math.cos(angle * Math.PI / 180.0),  sz * Math.sin(angle * Math.PI / 180.0),  -z_s,
		-sz * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0),  -z_s,

		-sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0),  z_f,
		-sz * Math.cos(angle * Math.PI / 180.0),  sz * Math.sin(angle * Math.PI / 180.0),  z_f,
		-sz * Math.cos(angle * Math.PI / 180.0),  sz * Math.sin(angle * Math.PI / 180.0),  -z_s,
		-sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0),  -z_s,

		-sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0),  z_f,
		-sz * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		-sz * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  -z_s,
		-sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0),  -z_s,

		-sz * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		 sz * Math.sin(angle * Math.PI / 180.0),  -sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		 sz * Math.sin(angle * Math.PI / 180.0),  -sz * Math.cos(angle * Math.PI / 180.0),  -z_s,
		-sz * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  -z_s,

		 sz * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		 sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0),  z_f,
		 sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0),  -z_s,
		 sz * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  -z_s,

		 sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0), z_f,
		 sz * Math.cos(angle * Math.PI / 180.0), sz * Math.sin(angle * Math.PI / 180.0), z_f,
		 sz * Math.cos(angle * Math.PI / 180.0), sz * Math.sin(angle * Math.PI / 180.0), -z_s,
		 sz * Math.cos(angle * Math.PI / 180.0), -sz * Math.sin(angle * Math.PI / 180.0), -z_s,

		 sz * Math.cos(angle * Math.PI / 180.0), sz * Math.sin(angle * Math.PI / 180.0), z_f,
		 sz * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), z_f,
		 sz * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), -z_s,
		 sz * Math.cos(angle * Math.PI / 180.0), sz * Math.sin(angle * Math.PI / 180.0), -z_s,
	];

	// Now pass the list of positions into WebGL to build the
	// shape. We do this by creating a Float32Array from the
	// JavaScript array, then use it to fill the current buffer.
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

	const textureCoordBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, textureCoordBuffer);

	const textureCoordinates = [
		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,

		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,

		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,

		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,

		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,

		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,

		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,

		0.0,  0.0,
		1.0,  0.0,
		1.0,  1.0,
		0.0,  1.0,
	];

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoordinates), gl.STATIC_DRAW);

	// Now set up the colors for the faces. We'll use solid colors
	// for each face.
	// Now send the element array to GL
	const normalBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);

	var angle = 45.0;

	const vertexNormals = [
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,

		0.0,  -1.0, 0.0,
		0.0,  -1.0, 0.0,
		0.0,  -1.0, 0.0,
		0.0,  -1.0, 0.0,

		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,

		-1.0, 0.0,  0.0,
		-1.0, 0.0,  0.0,
		-1.0, 0.0,  0.0,
		-1.0, 0.0,  0.0,

		-Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,
		-Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,
		-Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,
		-Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,

		Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,
		Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,
		Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,
		Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,

		-Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,
		-Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,
		-Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,
		-Math.sin(angle * Math.PI / 180.0),  -Math.sin(angle * Math.PI / 180.0),  0.0,

		Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,
		Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,
		Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,
		Math.sin(angle * Math.PI / 180.0),  Math.sin(angle * Math.PI / 180.0),  0.0,
	];

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexNormals), gl.STATIC_DRAW);


  	const indexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);

	// This array defines each face as two triangles, using the
	// indices into the vertex array to specify each triangle's
	// position.
	const indices = [
		0,  1,  2,      0,  2,  3,
		4,  5,  6,      4,  6,  7,
		8,  9,  10,     8,  10, 11,
		12, 13, 14,    12,  14, 15,
		16, 17, 18,    16,  18, 19,
		20, 21, 22,    20,  22, 23,
		24, 25, 26,    24,  26, 27,
		28, 29, 30,    28,  30, 31,
	];

	// Now send the element array to GL
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);

	return {
		position: positionBuffer,
		normal: normalBuffer,
		textureCoord: textureCoordBuffer,
		indices: indexBuffer,
	};
}

function loadTexture(gl, url) {
	const texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, texture);

	// Because images have to be download over the internet
	// they might take a moment until they are ready.
	// Until then put a single pixel in the texture so we can
	// use it immediately. When the image has finished downloading
	// we'll update the texture with the contents of the image.
	const level = 0;
	const internalFormat = gl.RGBA;
	const width = 1;
	const height = 1;
	const border = 0;
	const srcFormat = gl.RGBA;
	const srcType = gl.UNSIGNED_BYTE;
	const pixel = new Uint8Array([0, 0, 255, 255]);  // opaque blue
	gl.texImage2D(gl.TEXTURE_2D, level, internalFormat, width, height, border, srcFormat, srcType, pixel);

	const image = new Image();
	image.onload = function() {
		gl.bindTexture(gl.TEXTURE_2D, texture);
		gl.texImage2D(gl.TEXTURE_2D, level, internalFormat, srcFormat, srcType, image);

		// WebGL1 has different requirements for power of 2 images
		// vs non power of 2 images so check if the image is a
		// power of 2 in both dimensions.
		if (isPowerOf2(image.width) && isPowerOf2(image.height)) {
			// Yes, it's a power of 2. Generate mips.
			gl.generateMipmap(gl.TEXTURE_2D);
		} else {
			// No, it's not a power of 2. Turn of mips and set
			// wrapping to clamp to edge
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		}
	};
	image.src = url;

	return texture;
}

function isPowerOf2(value) {
	return (value & (value - 1)) == 0;
}

function drawSceneTunnel(gl, programInfo, buffers, texture, deltaTime, now, angle) {
	gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
	gl.clearDepth(1.0);                 // Clear everything
	gl.enable(gl.DEPTH_TEST);           // Enable depth testing
	gl.depthFunc(gl.LEQUAL);            // Near things obscure far things

	// Clear the canvas before we start drawing on it.

	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	// Create a perspective matrix, a special matrix that is
	// used to simulate the distortion of perspective in a camera.
	// Our field of view is 45 degrees, with a width/height
	// ratio that matches the display size of the canvas
	// and we only want to see objects between 0.1 units
	// and 100 units away from the camera.

	const fieldOfView = 45 * Math.PI / 180;   // in radians
	const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
	const zNear = 0.1;
	const zFar = 250.0;
	const projectionMatrix = mat4.create();

	// note: glmatrix.js always has the first argument
	// as the destination to receive the result.
	mat4.perspective(projectionMatrix, fieldOfView, aspect, zNear, zFar);

	// Set the drawing position to the "identity" point, which is
	// the center of the scene.

	// Now move the drawing position a bit to where we want to
	// start drawing the square.
	for(var i=0; i<500; i++) {
		var angle_rad = 22.5 * Math.PI / 180.0;
		const modelViewMatrix = mat4.create();
		mat4.translate(modelViewMatrix, modelViewMatrix, [0, Math.cos(angle_rad) + Math.cos(angle_rad), -(i << 2) + now * 50]);
		mat4.rotate(modelViewMatrix, modelViewMatrix, angle_rad * ((i << 1) + 1), [0, 0, 1]);

		const normalMatrix = mat4.create();
		mat4.invert(normalMatrix, modelViewMatrix);
		mat4.transpose(normalMatrix, normalMatrix);


		// Tell WebGL how to pull out the positions from the position
		// buffer into the vertexPosition attribute
		{
			const numComponents = 3;
			const type = gl.FLOAT;
			const normalize = false;
			const stride = 0;
			const offset = 0;
			gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
			gl.vertexAttribPointer(
				programInfo.attribLocations.vertexPosition,
				numComponents,
				type,
				normalize,
				stride,
				offset
			);
			gl.enableVertexAttribArray(programInfo.attribLocations.vertexPosition);
		}

		{
			const numComponents = 2;
			const type = gl.FLOAT;
			const normalize = false;
			const stride = 0;
			const offset = 0;
			gl.bindBuffer(gl.ARRAY_BUFFER, buffers.textureCoord);
			gl.vertexAttribPointer(
				programInfo.attribLocations.textureCoord,
				numComponents,
				type,
				normalize,
				stride,
				offset
			);
			gl.enableVertexAttribArray(programInfo.attribLocations.textureCoord);
		}
		{
			const numComponents = 3;
			const type = gl.FLOAT;
			const normalize = false;
			const stride = 0;
			const offset = 0;
			gl.bindBuffer(gl.ARRAY_BUFFER, buffers.normal);
			gl.vertexAttribPointer(
				programInfo.attribLocations.vertexNormal,
				numComponents,
				type,
				normalize,
				stride,
				offset
			);
			gl.enableVertexAttribArray(programInfo.attribLocations.vertexNormal);
		}

		// Tell WebGL which indices to use to index the vertices
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, buffers.indices);

		// Tell WebGL to use our program when drawing

		gl.useProgram(programInfo.program);

		// Set the shader uniforms

		gl.uniformMatrix4fv(
			programInfo.uniformLocations.projectionMatrix,
			false,
			projectionMatrix
		);
		gl.uniformMatrix4fv(
			programInfo.uniformLocations.modelViewMatrix,
			false,
			modelViewMatrix
		);
		gl.uniformMatrix4fv(
			programInfo.uniformLocations.normalMatrix,
			false,
			normalMatrix
		);

		gl.activeTexture(gl.TEXTURE0);

		// Bind the texture to texture unit 0
		gl.bindTexture(gl.TEXTURE_2D, texture);
		gl.uniform1i(programInfo.uniformLocations.uSampler, 0);

		{
			const vertexCount = 48;
			const type = gl.UNSIGNED_SHORT;
			const offset = 0;
			gl.drawElements(gl.TRIANGLES, vertexCount, type, offset);
		}
	}
}
