var getFragmentShader = function () {
	const fsSource = `
		varying highp vec2 vTextureCoord;
		varying highp vec3 vLighting;

		uniform sampler2D uSampler;

		void main(void) {
			highp vec4 texelColor = texture2D(uSampler, vTextureCoord);

			gl_FragColor = vec4(texelColor.rgb * vLighting, texelColor.a);
		}
	`;
	return fsSource;
}

var getFragmentShaderGreyScale = function () {

	const fsSource = `
		#ifdef GL_ES
		precision mediump float;
		#endif
		varying highp vec2 vTextureCoord;
		varying highp vec3 vLighting;
		uniform sampler2D uSampler;
		void main(void) {
			highp vec4 texelColor = texture2D(uSampler, vTextureCoord);
			vec3 color = texelColor.rgb;
			float gray = (color.r + color.g + color.b) / 3.0;
			vec3 gr = vec3(gray);
			gl_FragColor = vec4(gr * vLighting, texelColor.a);
		}
	`;

	return fsSource;
}

