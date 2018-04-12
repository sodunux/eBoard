case 0xF0:
      IIC_Start(1);
			break;
    case 0xF1:
      IIC_Stop(1);
			break;
    case 0xF2:
      IIC_Ack(1);
			break;
    case 0xF3:
      IIC_NoAck(1);
			break;
		case 0xF4:
      IIC_WaitAck(1);
			break;
    case 0xF5:
      IIC_Delay();
			break;
    case 0xF6:
      iic_tmp=IIC_ReadByte(1);
      break;
		default:
      IIC_SendByte(1,readvalue);
			break;