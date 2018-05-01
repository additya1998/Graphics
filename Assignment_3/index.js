// Configurations
var angle_move = 10;
var countTypes = [0, 2, 3, 15];
var level = 1, score = 0, cur_time = 0, obstacle_passed = 0;
var greyScale = false;
var powerups_left = 3, powerup_time = 0;

var rotate_offset = 0;

// mouse bindings
Mousetrap.bind('right', function() {
	rotate_offset = rotate_offset - angle_move;
});
Mousetrap.bind('left', function() {
	rotate_offset = rotate_offset + angle_move;
});
Mousetrap.bind('g', function() {
	if(greyScale) greyScale = false;
	else greyScale = true;
});
Mousetrap.bind('p', function() {
	if(powerups_left){
		--powerups_left;
		powerup_time += 100;
	}
});


main();

//
// Start here
//
function main() {

	// document.getElementById('backaudio').play()

	const canvas = document.querySelector('#glcanvas');
	const gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl');

	// If we don't have a GL context, give up now
	if (!gl) {
		alert('Unable to initialize WebGL. Your browser or machine may not support it.');
		return;
	}

	// Vertex shader program
	const vsSource = getVertexShader();

	// Fragment shader program
	const fsSource = getFragmentShader();
	const fsSourceGS = getFragmentShaderGreyScale();

	// Initialize a shader program; this is where all the lighting
	// for the vertices and so forth is established.
	const shaderProgram = initShaderProgram(gl, vsSource, fsSource);
	const shaderProgramGS = initShaderProgram(gl, vsSource, fsSourceGS);

	// Collect all the info needed to use the shader program.
	// Look up which attributes our shader program is using
	// for aVertexPosition, aVevrtexColor and also
	// look up uniform locations.
	const programInfo = {
		program: shaderProgram,
		attribLocations: {
			vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
			vertexNormal: gl.getAttribLocation(shaderProgram, 'aVertexNormal'),
			textureCoord: gl.getAttribLocation(shaderProgram, 'aTextureCoord'),
		},
		uniformLocations: {
			projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'),
			modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'),
			normalMatrix: gl.getUniformLocation(shaderProgram, 'uNormalMatrix'),
			uSampler: gl.getUniformLocation(shaderProgram, 'uSampler'),
		},
	};

	const programInfoGS = {
		program: shaderProgramGS,
		attribLocations: {
			vertexPosition: gl.getAttribLocation(shaderProgramGS, 'aVertexPosition'),
			vertexNormal: gl.getAttribLocation(shaderProgramGS, 'aVertexNormal'),
			textureCoord: gl.getAttribLocation(shaderProgramGS, 'aTextureCoord'),
		},
		uniformLocations: {
			projectionMatrix: gl.getUniformLocation(shaderProgramGS, 'uProjectionMatrix'),
			modelViewMatrix: gl.getUniformLocation(shaderProgramGS, 'uModelViewMatrix'),
			normalMatrix: gl.getUniformLocation(shaderProgramGS, 'uNormalMatrix'),
			uSampler: gl.getUniformLocation(shaderProgramGS, 'uSampler'),
		},
	};


	const tunnelTexture = loadTexture(gl, 'tunnel.jpeg');
	const obstacleTexture = [loadTexture(gl, 'o1.jpeg'), loadTexture(gl, 'o2.jpeg'), loadTexture(gl, 'o3.jpeg')];

	var tunnel = initBuffersTunnel(gl);
	var obstacles = [];
	var obstacle_offset = 150;
	console.log(countTypes);
	for(var i=0; i<countTypes.length; ++i){
		console.log(i, countTypes[i]);
		for(var j=1; j<=countTypes[i]; ++j){
			obstacles.push(initBuffersObstacle(gl, obstacle_offset, i));
			obstacle_offset = obstacle_offset + 50 * (4 - i);
		}
	}

	var cur_time = 0, count = 0;


	// Draw the scene repeatedly
	function render(now) {
		now *= 0.001;  // convert to seconds
		const deltaTime = now - cur_time;
		cur_time = now;
		
		if(greyScale) drawSceneTunnel(gl, programInfoGS, tunnel, tunnelTexture, deltaTime, now, rotate_offset);
		else drawSceneTunnel(gl, programInfo, tunnel, tunnelTexture, deltaTime, now, rotate_offset);

		for(var i=0; i<obstacles.length; i++) {
			if(obstacles[i].active){
				var function_return = drawSceneObstacle(gl, programInfo, obstacles[i], obstacleTexture[obstacles[i].type - 1], deltaTime, now, rotate_offset);
				if(function_return == 0);
				else if(function_return == 1){
					if(powerup_time > 0);
					else exit(0);
				}
				else ++obstacle_passed;				
			}
		}
		requestAnimationFrame(render);

		score = score + 1;
		level = 0;
		var cum_count = 0;
		for(var i=0; i<countTypes.length; ++i){
			cum_count = cum_count + countTypes[i];
			if(obstacle_passed >= cum_count) ++level;
			else break;
		}
		console.log(level);

		if(powerup_time > 0) --powerup_time;
		document.title = "Score: " + score + ", Level: " + level + ", PowerUp left: " + powerups_left + ", PowerUp time: " + powerup_time;
	}
	requestAnimationFrame(render);
}