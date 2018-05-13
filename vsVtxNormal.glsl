#version 150
in vec4 vPosition;
in vec3 vNormal;
out vec4 color;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 vColor;

void main()
{
    gl_Position = Projection * ModelView * vPosition;
	if( vColor.x == -1.0f && vColor.y == -1.0f && vColor.z == -1.0f ) {
		if( vNormal.x > 0 || vNormal.y > 0 || vNormal.z > 0) color = vec4(vNormal, 1.0);
		else color = vec4(-vNormal*0.5, 1.0);
	}
	else color = vColor;
}
