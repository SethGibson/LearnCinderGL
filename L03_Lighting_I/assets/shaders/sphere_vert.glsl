/* SHADERS!! ------------------------------------------------------------------------------------------------------
	Ok so you've probably heard a ton about shaders in your readings and discussions and such
	or you may be familiar with the idea of shaders from other 3d contexts. In OpenGL/DirectX/Cinder/Realtime 3d graphics land,
	a shader is basically just a program that runs specifically on the GPU and defines part of the
	rendering pipeline. You may have heard the terms "fixed function pipeline" vs "programmable pipeline",
	well, in dark times, the OpenGL rendering pipeline was, erm, fixed, which basically meant that you could
	only define so much of the input and output. Now in Modern OpenGL, we can pretty much control every step
	of the rendering process with shaders, so we can control vertices, fragments, primitives, the horizontal,
	the vertical, etc. For these tutorials we'll mainly be focusing on vertices and fragments, so here we go...
-----------------------------------------------------------------------------------------------------------------*/

// this is similar to a c++ preprocessor define, wherein we tell the shader compiler certain things
// about our shader prior to the main compilation. Here we set the GLSL version, which gives us
// access to certain features
#version 150	

/* Communicating Between The App/Context And The Shaders: Uniforms, Ins, and Outs
	As you've probably guessed, there are a few things we need to be able to do with our shaders.
	If you think about that term "pipeline", here it's used for a good reason. Think of it as
	passing data from the application, through each shader, then back to the application, and eventually
	the screen. So we need some way to get data from the app into the shader, then from one shader to the next
	shader. Enter uniforms, ins, and outs.

	Now, for the most part, when we're working with data in a shader, the actual types you're already familiar with,
	e.g. floats, ints, vectors, etc. To tell the pipeline what we're using a specific variable of type for, we add what's
	called a "storage qualifier" or "type qualifier". Prior to GLSL version 150, you would've seen things like uniform, attribute, varying, etc.
	Nowadays, things have been simplified a bit and we use the qualifiers "uniform", "in", and "out" (note that if we hadn't
	added the above version statement, we wouldn't be able to use "uniform", "in", and "out" and instead would have to use
	"uniform", "varying", and "attribute". So what does each one do?

	UNIFORM (UNIFORM)
		Think of a uniform like a global variable, that is, once a shader program starts to run, the uniform remains the same
		for the entire execution of the program. Another way to think of it is as a piece of data that doesn't change per
		component in the shader stage, so in a vertex shader, a uniform value will be the same for every vertex in the
		object that's using said shader. Uniforms often times come from the calling app, but you can define and declare a uniform
		within the shader (check out sphere_frag.glsl for an example). If you're porting pre-existing shader code from an earlier
		version of glsl, "uniform" == "uniform"

	IN (ATTRIBUTE)
		In variables are data that are passed into a shader from the preceeding stage in the pipeline, and can be different
		per component. For example, in a vertex shader, an in variable is data coming into the app, and the data is applied per-vertex
		rather than per instance of the shader as with a uniform (which is why in the case of vertex shaders, you'll hear ins referred
		to as "vertex attributes"). An example of a vertex in would be per vertex color. If you're porting pre-existing shader code from an earlier
		version of glsl, "in" == "attribute"

	OUT (VARYING)
		Out variables are data that are passed from the current pipeline stage to the next stage, for example, a fragment shader
		defines a single out of type vec4 which goes from the fragment shader to the rendering context, which could be the screen
		in the case of drawing an object, or could be a separate frame buffer in the case of full screen effects (we'll explore those in
		Lesson 10). One thing to keep in mind is that an out from one stage will have the same variable name as the corresponding
		in in the next stage, for example, here in sphere_vert, we define "out vec3 Normal", which maps to "in vec3 Normal" in
		sphere_frag. If you're porting pre-existing shader code from an earlier	version of glsl, "out" == "varying"
*/

/* Cinder Shader Variables
	99% of the time, there are certain bits of data that a shader is going to expect from the application to get its job done,
	things like vertex positions, normals, uvs, matrices, etc. Ordinarily you would have to set these up in your host app
	and pass them through to the shader, but Cinder handles many of these for you. Anytime you see a shader variable that starts 
	with "ci", that's one of our Cinder defined vars. You can find the full list of these in cinder/source/gl/GlslProg.cpp.
*/

// Alright so, with all that context set, let's get to the specifics of our shader:

/* Uniforms:
	
*/
uniform mat4 ciModelViewProjection;
uniform mat4 ciModelMatrix;

/* Ins:
*/
in vec4 ciPosition;
in vec4 ciNormal;

/* Outs:
*/
out vec3 Normal;
out vec3 WorldPos;


void main()
{
	WorldPos = vec3(ciModelMatrix*ciPosition);
	Normal = vec3(ciModelMatrix*ciNormal);
	gl_Position = ciModelViewProjection * ciPosition;
}