#version 150
in  vec4 vPosition;
out vec4 color;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 vColor;

void main()
{
    gl_Position = Projection * ModelView * vPosition;
	if( vColor.x == -1.0f && vColor.y == -1.0f && vColor.z == -1.0f ) {
		color = vec4( ( 1.0 + vPosition.x ) / 2.0, ( 1.0 + vPosition.y ) / 2.0,( 1.0 + vPosition.z ) / 2.0, 1.0 );
	}
	else if( vColor.x == -1.0f && vColor.y == 0.0f && vColor.z == 0.0f ) {
		color = vec4( ( 1.0 + vPosition.y ) / 2.0, ( 1.0 + vPosition.y ) / 2.0,( 0.5 + vPosition.z ) / 2.0, 1.0 );
	}
	else if( vColor.x == 0.0f && vColor.y == -1.0f && vColor.z == 0.0f ) {
		color = vec4( ( vPosition.x ) / 2.0, ( 1.0 + vPosition.y ) / 2.0,( 1.0 + vPosition.z ) / 2.0, 1.0 );
	}
	else if( vColor.x == 0.0f && vColor.y == 0.0f && vColor.z == -1.0f ) {
		color = vec4( ( 1.0 + vPosition.z ) / 2.0, ( vPosition.y ) / 2.0,( 1.0 + vPosition.y ) / 2.0, 1.0 );
	}
	else {
		color = vColor;
	}
}