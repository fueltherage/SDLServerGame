#pragma once 
#include <stdio.h>
#include <math.h>

#ifndef VERY_SMALL
#define VERY_SMALL 1.0e-9
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS ( M_PI / 180.0f)
#endif



		///
		/// Vector and Quaternion definitions
		///
struct Vector2D {
	float  x,y;	///  Structures are default public!
		
	float Vector2D::length()
	{	
		return ::sqrt(x * x + y * y);
	}

	Vector2D normalized()
	{
		if(length()!=0){
			return Vector2D(x/length(),y/length());
		}
		else{ return Vector2D(0.0,0.0);}
	}

	//Dot product by Greg Vincze :D
	float DotProduct(Vector2D axis) {
		return x*axis.x+y*axis.y;
	}

	void Perpendicular () {
		float tempX = x;
		x = y;
		y = -tempX;
	}

	void Reverse () {
		x = -x;
		y = -y;
	}
		
	void Vector2D::Load(float _x, float _y) {
		x = _x;
		y = _y;
	}

	/// Here's a set of constructors
	Vector2D( float s = float(0.0) ){ Load(s,s); }
	Vector2D( float _x, float _y){ Load(_x,_y); }

	/// In this context "const" means I promise not to modify anything v points to   
	Vector2D( const Vector2D& v ) { Load(v.x,v.y); }


	/// I know, I hate operator overloading but a few make sense!! - SSF

	/// Now we can use the vector like an array butwe'll need two overloads
	const float operator [] ( int index) const {  /// This one is for reading the Vector as if where an array
		return *(&x + index); 
	}
	float& operator [] ( int index ) { /// This one is for writing to the Vector as if where an array - check out the term lvalue
		return *(&x + index); 
	}
	

	/// Add two Vectors
	Vector2D operator + ( const Vector2D& v ) const { 
		return Vector2D( x + v.x, y + v.y); 
	}

	/// Increment by a scalar
	Vector2D& operator += ( const Vector2D& v ){ 
		x += v.x;  
		y += v.y;  		
		return *this; 
	}

	/// Take the negative of a Vector
	Vector2D operator - () const  { 
		return Vector2D( -x, -y); 
	}   

	/// Subtract two Vectors
	Vector2D operator - ( const Vector2D& v ) const { 
		return Vector2D(x - v.x, y - v.y); 
	}

	/// Decrement by a scalar
	Vector2D& operator -= ( const Vector2D& v ){ 
		x -= v.x;  
		y -= v.y;  
			
		return *this; }

	/// Multiply a Vector by a scalar
	Vector2D  operator *  ( const float s ) const { 
		return Vector2D(s*x, s*y); 
	}

	/// Multiply a Vector by a scalar and assign it to its self
	Vector2D& operator *= ( const float s ) { 
		x *= s; 
		y *= s;  			
		return *this; }

	/// Multiply a scaler by a Vector  - Ha! It's the scalar first then the vector
	/// Overloaded and a friend!!! Ouch! It's the only way to make it work with a scalar first! 
	friend Vector2D operator * ( const float s, const Vector2D& v ) { 
		return v * s; 
	}

	/// Multiply two Vectors
	Vector2D  operator *  ( const Vector2D& v ) const{ 
		return Vector2D( x*v.x, y*v.y); 
	}

	Vector2D& operator *= ( const Vector2D& v ){ 
		x *= v.x;  
		y *= v.y;  
			
		return *this; 
	}

	/// Divide by a scalar - Watch for divide by zero issues
	Vector2D operator / ( const float s ) const {
#ifdef DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
	if ( fabs(s) < VERY_SMALL ) {
		std::string errorMsg("Divide by nearly zero! ");
		throw errorMsg;
	}
#endif
	float r = 1.0f / s;
	return *this * r;
	}

	Vector2D& operator /= ( const float s ) {
#ifdef DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
	if ( std::fabs(s) < VERY_SMALL ) {
		std::string errorMsg("Divide by nearly zero! ");
		throw errorMsg;
#endif // DEBUG
	float r = 1.0f / s;
	*this *= r;
	return *this;
	}

	void Print() { 
		printf("%f %f\n", x,y);		  
	}
};
		///
		/// Quaternions definitions 
		///
		/*
	struct Quaternions {
		float  x;	///
		float  y;	///  Structures are default public!
		float  z;	/// 
		float  w;

		/// Here's a set of constructors
		Quaternions( float s = float(0.0) ) : x(s), y(s), z(s), w(s){}
		Quaternions( float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
		Quaternions( const Quaternions& v ) { 
			x = v.x;  
			y = v.y;  
			z = v.z; 
			w = v.w;
		}

   
		/// See Vector3 definition 
		float& operator [] ( int index ) { 
			return *(&x + index); 
		}
		const float operator [] ( int i ) const { 
			return *(&x + i); 
		}

		/// See Vector3 definition 
		Quaternions operator + ( const Quaternions& v ) const { 
			return Quaternions( x + v.x, y + v.y, z + v.z, w + v.w ); 
		}

		/// See Vector3 definition 
		Quaternions& operator += ( const Quaternions& v ){ 
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this; 
		}

		//// See Vector3 definition 
		Quaternions operator - () const  { 
			return Quaternions( -x, -y, -z, -w );
		}   

		/// See Vector3 definition 
		Quaternions operator - ( const Quaternions& v ) const { 
			return Quaternions( x - v.x, y - v.y, z - v.z, v.w - w);
		}

		/// See Vector3 definition 
		Quaternions& operator -= ( const Quaternions& v ){ 
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		/// See Vector3 definition 
		Quaternions operator * ( const float s ) const { 
			return Quaternions( s*x, s*y, s*z, s*w);
		}

		Quaternions& operator *= ( const float s ) { 
			x *= s;
			y *= s;
			z *= s;
			w*= s;
			return *this;
		}

		/// See Vector3 definition 
		 friend Quaternions operator * ( const float s, const Quaternions& v ) { 
			 return v * s; 
		 }

		/// Multiply two Vectors
		Quaternions operator * ( const Quaternions& v ) const{ 
			return Quaternions( x*v.x, y*v.y, z*v.z, w*v.w);
		}
		Quaternions& operator *= ( const Vector& v ){ 
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		Quaternions operator / ( const float s ) const {
	#ifdef DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
		if ( std::fabs(s) < VERY_SMALL ) {
			std::string errorMsg("Divide by nearly zero! ");
			throw errorMsg;
		}
	#endif
		float r = 1.0f / s;
		return *this * r;
		}

		Quaternions& operator /= ( const float s ) {
	#ifdef DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
		if ( std::fabs(s) < VERY_SMALL ) {
			std::string errorMsg("Divide by nearly zero! ");
			throw errorMsg;
	#endif // DEBUG

		float r = 1.0f / s;
		*this *= r;
		return *this;
		}


		///
		/// Type conversion operators 
		/// Extra credit if you can figure this stuff out
		///
		operator const float* () const { 
			return static_cast<const float*>( &x );
		}
		operator float* () { 
			return static_cast<float*>( &x );
		}
		

	};
		*/
