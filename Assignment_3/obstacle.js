function initBuffersObstacle(gl, z_offset, type) {

	// Create a buffer for the cube's vertex positions.

	const positionBuffer = gl.createBuffer();

	// Select the positionBuffer as the one to apply buffer
	// operations to from here out.

	gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

	// Now create an array of positions for the cube.
	var sz = 5, d = 1.5, angle = 22.5, z_f = 0.0, z_s = 1.5;
	const positions = [
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0),  sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0),  sz * Math.cos(angle * Math.PI / 180.0),  -z_s,
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0),  -z_s,

		-(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0),  -sz * Math.cos(angle * Math.PI / 180.0),  z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0),  -sz * Math.cos(angle * Math.PI / 180.0),  -z_s,
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0),  -z_s,

		-(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), z_f,
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), z_f,

		-(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), -z_s,
		(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), -z_s,
		(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), -z_s,
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), -z_s,

		(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), z_f,
		(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), -z_s,
		(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), -z_s,

		-(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), z_f,
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), z_f,
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), -sz * Math.cos(angle * Math.PI / 180.0), -z_s,
		-(sz - d) * Math.sin(angle * Math.PI / 180.0), sz * Math.cos(angle * Math.PI / 180.0), -z_s,
	];

	// Now pass the list of positions into WebGL to build the
	// shape. We do this by creating a Float32Array from the
	// JavaScript array, then use it to fill the current buffer.

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

	const normalBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);

	const vertexNormals = [
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,
		0.0,  0.0,  1.0,

		0.0,  0.0, -1.0,
		0.0,  0.0, -1.0,
		0.0,  0.0, -1.0,
		0.0,  0.0, -1.0,

		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,

		0.0, -1.0,  0.0,
		0.0, -1.0,  0.0,
		0.0, -1.0,  0.0,
		0.0, -1.0,  0.0,

		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,

		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0
	];

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexNormals), gl.STATIC_DRAW);

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
	];

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoordinates), gl.STATIC_DRAW);

	// Build the element array buffer; this specifies the indices
	// into the vertex arrays for each face's vertices.

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
	];

	// Now send the element array to GL

	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);

	var angle_ob = Math.ceil(Math.random() * 360) % 360;
	var rotate_direction = Math.floor(Math.random() * 2);
	return {
		position: positionBuffer,
		normal: normalBuffer,
		textureCoord: textureCoordBuffer,
		indices: indexBuffer,
		textureCoord: textureCoordBuffer,
		distance: z_offset,
		type: type,
		offset: angle_ob,
		rotate_direction: rotate_direction,
		active: true
	};
}

function drawSceneObstacle(gl, programInfo, obstacle, texture, deltaTime, now, angle) {

	var return_value = 0;
	var z_offset = obstacle.distance;
	var type = obstacle.type;
	var offset = obstacle.offset;
	var rotate_direction = obstacle.rotate_direction;

	// gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
	// gl.clearDepth(1.0);                 // Clear everything
	gl.enable(gl.DEPTH_TEST);           // Enable depth testing
	gl.depthFunc(gl.LEQUAL);            // Near things obscure far things

	// Clear the canvas before we start drawing on it.
	// Create a perspective matrix, a special matrix that is
	// used to simulate the distortion of perspective in a camera.
	// Our field of view is 45 degrees, with a width/height
	// ratio that matches the display size of the canvas
	// and we only want to see objects between 0.1 units
	// and 100 units away from the camera.

	const fieldOfView = 45 * Math.PI / 180;   // in radians
	const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
	const d_add = now * 50;
	const cur_pos = -z_offset + d_add;
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

	const modelViewMatrix = mat4.create();
	mat4.translate(modelViewMatrix, modelViewMatrix, [0, 3 * Math.cos(22.5 * Math.PI / 180.0), cur_pos]);
	
	var z_angle = angle + offset;
	// moving obstacle
	z_angle = z_angle + ((type - 1) * d_add);
	z_angle = Math.PI * z_angle / 180.0;

	z_angle = (rotate_direction == 0) ? z_angle : -z_angle;

	// check collission
	if((cur_pos >= 0.0 && cur_pos <= 1.5)) {
		var angle = 22.5 * Math.PI / 180;
		if((Math.abs(3.0 * Math.cos(angle) * Math.sin(z_angle)) <= (3.0 * Math.sin(angle)))) return_value = 1;
		else return_value = 2;
		obstacle.active = false;
	}

	mat4.rotate(modelViewMatrix, modelViewMatrix, z_angle, [0, 0, 1]);

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
		gl.bindBuffer(gl.ARRAY_BUFFER, obstacle.position);
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
		const num = 2; // every coordinate composed of 2 values
		const type = gl.FLOAT; // the data in the buffer is 32 bit float
		const normalize = false; // don't normalize
		const stride = 0; // how many bytes to get from one set to the next
		const offset = 0; // how many bytes inside the buffer to start from
		gl.bindBuffer(gl.ARRAY_BUFFER, obstacle.textureCoord);
		gl.vertexAttribPointer(programInfo.attribLocations.textureCoord, num, type, normalize, stride, offset);
		gl.enableVertexAttribArray(programInfo.attribLocations.textureCoord);
	}
	{
		const numComponents = 3;
		const type = gl.FLOAT;
		const normalize = false;
		const stride = 0;
		const offset = 0;
		gl.bindBuffer(gl.ARRAY_BUFFER, obstacle.normal);
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
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, obstacle.indices);

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
		)
	;

	gl.activeTexture(gl.TEXTURE0);

	// Bind the texture to texture unit 0
	gl.bindTexture(gl.TEXTURE_2D, texture);

	// Tell the shader we bound the texture to texture unit 0
	gl.uniform1i(programInfo.uniformLocations.uSampler, 0);

	{
		const vertexCount = 36;
		const type = gl.UNSIGNED_SHORT;
		const offset = 0;
		gl.drawElements(gl.TRIANGLES, vertexCount, type, offset);
	}

	return return_value;
}
