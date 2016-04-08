using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Text;

public class ChatClient : MonoBehaviour {
	
	private byte[] data = new byte[1];
	private NetworkStream socketStream;
	private TcpClient tcpClient;
	
	private string read_data = "";
	private ArrayList commandList = new ArrayList();
	
	public string userName = "";
	public string auth_key = "1";
	public string chatIP = "78.47.177.21";
	public int chatPort = 57051;
	
	public delegate void ConnectionEventHandler(bool isConnected);
    public event ConnectionEventHandler onConnectionChanged;
	
	public delegate void MessageEventHandler(uint channelId, string name, string text);
    public event MessageEventHandler onMessage;
	
	public delegate void PrivateMessageEventHandler(string name, string text);
    public event PrivateMessageEventHandler onPrivateMessage;
	
	public delegate void ChannelCreatedEventHandler(string name, uint id);
    public event ChannelCreatedEventHandler onChannelCreated;
	
	public delegate void ChannelLeaveEventHandler(uint id);
    public event ChannelLeaveEventHandler onChannelLeave;
	
	void Awake() 
	{
		Application.runInBackground = true;
    }
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (socketStream != null)
		if (socketStream.DataAvailable)
		if (socketStream.CanRead)
		{
			data = new byte [1024];
			int recv = socketStream.Read(data,0,data.Length);
			//print ("raw lentgh: " + recv);
			if(recv > 0)
			{
				string txt = Encoding.UTF8.GetString(data, 0, recv);
				//print ("parsed lentgh: " + txt.Length);
				//print (txt);
				read_data += txt;
				parseChatProtocol();
			}
		}
	}
	
	private void parseChatProtocol()
	{
		string[] lines = read_data.Split('\n');
		read_data = "";
		commandList.AddRange(lines);
		commandList.Remove("");
		for (;commandList.Contains(".");)
		{
			ArrayList one_cmd = new ArrayList();
			ArrayList copy = new ArrayList(commandList);
			while (copy.Count > 0)
			{
				string first = copy[0].ToString();
				copy.RemoveAt(0);
				if (first != "."){
					one_cmd.Add(first);
				}
				else{
					if (copy.Count == 0)
					{
						commandList.Clear();
					}
					else{
						commandList = new ArrayList(copy);
					}
				}
			}
			dispatch(one_cmd);
		}
	}
	
	private void dispatch(ArrayList oneCmd)
	{
		if (oneCmd.Count == 0) return;
		string cmd = oneCmd[0].ToString();
		
		if (cmd == "CC")
		{
			if (oneCmd.Count == 3)
			{
				if (onChannelCreated != null)
				{
					onChannelCreated(oneCmd[1].ToString(), uint.Parse(oneCmd[2].ToString()));
				}
			}
		}
		else if (cmd == "LC")
		{
			if (oneCmd.Count == 2)
			{
				if (onChannelLeave != null)
				{
					onChannelLeave(uint.Parse(oneCmd[1].ToString()));
				}
			}
		}
		else if (cmd == "SM")
		{
			if (oneCmd.Count == 4)
			{
				if (onMessage != null)
				{
					onMessage(uint.Parse(oneCmd[1].ToString()), oneCmd[2].ToString(), oneCmd[3].ToString());
				}
			}
		}
	}
	
	public void SendMess(uint channelsId, string text)
	{
		string cmd = "SM\n" + channelsId.ToString() + "\n" + text + "\n.";
		Send(cmd);
	}
	
	public void CreateChannel(string name)
	{
		string cmd = "CC\n" + name + "\n.";
		Send(cmd);
	}
	
	public void LeaveeChannel(string name)
	{
		string cmd = "LC\n" + name + "\n.";
		Send(cmd);
	}
	
	public void Auth()
	{
		string cmd = "AU\n" + auth_key + "\n" + userName + "\n.";
		Send(cmd);
	}
	
	public void ConnectToServer()
	{
		if (tcpClient != null && tcpClient.Connected) return;
		try{
			tcpClient = new TcpClient(chatIP, chatPort);
			socketStream = tcpClient.GetStream();
			if (onConnectionChanged != null)
			{
				onConnectionChanged(true);
			}
			Auth();
		}catch(Exception e)
		{
			//print("Sending error: " + e.Message);
			if (onConnectionChanged != null)
			{
				onConnectionChanged(false);
			}
		}
	}
	
	private bool Send(string data)
	{
		if(tcpClient == null || tcpClient.Connected == false)
		{
			//print("no connection.. trying restore");
			//ConnectToServer();
			//return Send(data);
		}
		byte[] message = Encoding.UTF8.GetBytes(data);
		if(socketStream != null && socketStream.CanWrite)
		{
			try
			{
				socketStream.Write(message,0,message.Length);
				return true;
			}catch(Exception e)
			{
				//print("Sending error: " + e.Message);
			}
		}
		return false;
	}
	
	void OnApplicationQuit() 
	{
		if(tcpClient != null && tcpClient.Connected)
		{
			tcpClient.Close();
		}
    }
}
