using System;
using System.IO.Ports;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization;


namespace AVRConnector {

  public class ConnectionException : Exception, ISerializable {
	public ConnectionException(int code) : base(to_msg(code)) {
	}

	private static string to_msg(int code) {
	  switch(code) {
	  case STATUSCODE_FAIL : return "FAIL [1]"; 
	  case STATUSCODE_OK : return "OK [0]"; 
	  case STATUSCODE_NO_ACCESS : return "NO_ACCESS [3]"; 
	  case STATUSCODE_NO_SUCH_REGISTER : return "NO_SUCH_REGISTER [2]"; 
	  case STATUSCODE_NOT_IMPLEMENTED : return "NOT_IMPLEMENTED [5]"; 
	  case STATUSCODE_INVALID_OPCODE : return "INVALID_OPCODE [4]"; 
	  case STATUSCODE_PONG : return "PONG [6]"; 
	  default:
		return "unknown code"; 
	  }
	}

	private const int STATUSCODE_FAIL = 1;
	private const int STATUSCODE_OK = 0;
	private const int STATUSCODE_NO_ACCESS = 3;
	private const int STATUSCODE_NO_SUCH_REGISTER = 2;
	private const int STATUSCODE_NOT_IMPLEMENTED = 5;
	private const int STATUSCODE_INVALID_OPCODE = 4;
	private const int STATUSCODE_PONG = 6;

  }

  public class AVRConnection {
	private SerialPort port;

	private const int OPCODE_FN_write_float_register = 0;
	private const int OPCODE_FN_write_short_register = 1;
	private const int OPCODE_FN_read_float_register = 2;
	private const int OPCODE_FN_read_short_register = 3;
	private const int OPCODE_FN_set_led = 4;
	private const int OPCODE_FN_get_if_version = 5;
	private const int OPCODE_FN_ping = 6;
	private const int OPCODE_FN_ln5623_set_output = 7;


	private const int STATUSCODE_FAIL = 1;
	private const int STATUSCODE_OK = 0;
	private const int STATUSCODE_NO_ACCESS = 3;
	private const int STATUSCODE_NO_SUCH_REGISTER = 2;
	private const int STATUSCODE_NOT_IMPLEMENTED = 5;
	private const int STATUSCODE_INVALID_OPCODE = 4;
	private const int STATUSCODE_PONG = 6;

	public AVRConnection() {
	  port = new SerialPort();
	  port.BaudRate = 38400;
	  port.StopBits = StopBits.One;
	  port.ReadTimeout = 500;
	  port.Open();
	}

	  public Dictionary<string, object> read_all() {
		Dictionary<string, object> fields = new Dictionary<string, object>();

        fields["kp"] = get_kp();
        fields["ki"] = get_ki();
        fields["kd"] = get_kd();
        fields["abgas_v1190"] = get_abgas_v1190();
        fields["abgas_amp_gain"] = get_abgas_amp_gain();
        fields["vorlauf_v1190"] = get_vorlauf_v1190();
        fields["vorlauf_amp_gain"] = get_vorlauf_amp_gain();
        fields["temp_vorlauf"] = get_temp_vorlauf();
        fields["temp_abgas"] = get_temp_abgas();
        fields["temp_ambient"] = get_temp_ambient();
        fields["controller_output"] = get_controller_output();
	    return fields;
	  }
     public float get_kp() {
		float value;
        read_float_register(0, out value);
		return value;
	}
     public void set_kp(float value) {
        write_float_register(0, value);
	 }

     public float get_ki() {
		float value;
        read_float_register(1, out value);
		return value;
	}
     public void set_ki(float value) {
        write_float_register(1, value);
	 }

     public float get_kd() {
		float value;
        read_float_register(2, out value);
		return value;
	}
     public void set_kd(float value) {
        write_float_register(2, value);
	 }

     public float get_abgas_v1190() {
		float value;
        read_float_register(3, out value);
		return value;
	}
     public void set_abgas_v1190(float value) {
        write_float_register(3, value);
	 }

     public float get_abgas_amp_gain() {
		float value;
        read_float_register(4, out value);
		return value;
	}
     public void set_abgas_amp_gain(float value) {
        write_float_register(4, value);
	 }

     public float get_vorlauf_v1190() {
		float value;
        read_float_register(5, out value);
		return value;
	}
     public void set_vorlauf_v1190(float value) {
        write_float_register(5, value);
	 }

     public float get_vorlauf_amp_gain() {
		float value;
        read_float_register(6, out value);
		return value;
	}
     public void set_vorlauf_amp_gain(float value) {
        write_float_register(6, value);
	 }

     public short get_temp_vorlauf() {
		short value;
        read_short_register(0, out value);
		return value;
	}
     public short get_temp_abgas() {
		short value;
        read_short_register(1, out value);
		return value;
	}
     public short get_temp_ambient() {
		short value;
        read_short_register(2, out value);
		return value;
	}
     public short get_controller_output() {
		short value;
        read_short_register(3, out value);
		return value;
	}




  public void WriteByte(byte b) {
	Byte[] x = new Byte[1];
	x[0] = b;
	port.Write(x, 0, 1);
  }

  public void WriteShort(short b) {
	//
  }

  public void WriteUShort(ushort b) {
	//
  }

  public void WriteFloat(float b) {
	//	Byte[] x = new Byte[4];
	//	x[0] = b;
	//	port.Write(x, 0, 1);
	//tbd
  }


  public byte ReadByte() {
	Byte[] x = new Byte[1];
	port.Read(x, 0, 1);
	return x[0];
  }

  public short ReadShort() {
	return 0;
  }

  public ushort ReadUShort() {
	return 0;
  }

  public float ReadFloat() {
	return 0.0f;
  }



  public void write_float_register(byte id, float value) {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_write_float_register);
	WriteByte(id);
	WriteFloat(value);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
  }

  public void write_short_register(byte id, short value) {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_write_short_register);
	WriteByte(id);
	WriteShort(value);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
  }

  public void read_float_register(byte id, out float value) {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_read_float_register);
	WriteByte(id);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
    value = ReadFloat();
  }

  public void read_short_register(byte id, out short value) {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_read_short_register);
	WriteByte(id);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
    value = ReadShort();
  }

  public void set_led(byte on) {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_set_led);
	WriteByte(on);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
  }

  public void get_if_version(out byte version) {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_get_if_version);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
    version = ReadByte();
  }

  public void ping() {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_ping);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
  }

  public void ln5623_set_output(ushort value, byte dp) {

	port.DiscardInBuffer();
	port.DiscardOutBuffer();

	WriteByte(OPCODE_FN_ln5623_set_output);
	WriteUShort(value);
	WriteByte(dp);

	int status = ReadByte();
    if (status != STATUSCODE_OK) 
	  throw new ConnectionException(status);
  }


 }
}


