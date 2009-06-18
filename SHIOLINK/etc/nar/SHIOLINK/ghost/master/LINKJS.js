var loaddir = "";
var stdin  = WScript.StdIn;
var stdout = WScript.StdOut;
var stderr = WScript.StdErr;

//=============================================================
// ���N�G�X�g���P�؂�o���܂Ńf�[�^��ǂݍ��ށB
//=============================================================
function readRequest(){
  var request, line, ss;
  mode = 1;
  request        = new Object();
  request.method = null;
  request.item   = new Object();

  while (!stdin.AtEndOfStream){
    line = stdin.ReadLine();
    line = line.replace(/[ �@\t\r\n]*$/gim, "");
    stderr.WriteLine("get command:[" + line +"]");

    switch(mode){
    case 1: // *------- �R�}���h���[�h
      var cmd = line.substr(0,3);
      switch(cmd){
      case "*L:":
        loaddir = line.substr(3);
        continue;
      case "*U:":
        return null;
      case "*S:":
        // seq����
        stdout.WriteLine(line);
        mode = 2;
      }
      continue;

    case 2: // *------- request method��M���[�h
      switch(line){
      case "GET SHIORI/3.0":    request.method = "GET"; break;
      case "NOTIFY SHIORI/3.0": request.method = "NOTIFY"; break;
      default:
        return request;
      }
      mode = 3;
      continue;

    case 3: // *------- request item��M���[�h
      if(line.length == 0) return request;
      ss = line.split(": ");
      request.item[ss[0]] = ss[1];
    }
    continue;

  }
  return null;
}


//=============================================================
// �����֐��F�X
//=============================================================
function response(resid ,lines){
  var buf = "SHIORI/3.0 " + resid + "\r\n";
  buf = buf + "Charset: Shift_JIS" + "\r\n";
  buf = buf + "Sender: LINKJS" + "\r\n";
  buf = buf + lines;
  stderr.WriteLine(buf);// �C�W���̂悤��stderr�ɓf���܂���
  stdout.WriteLine(buf);
}

function response_bad_request(region){
  response(
    "400 Bad Request",
    "X-LINKJS-Reason: " + region + "\r\n");
}

function response_no_content(lines){
  response(
    "204 No Content",
    lines);
}

function response_ok(lines) {
  response(
    "200 OK",
    lines);
}


//=============================================================
// �C�x���g�o�^
//=============================================================
var event = new Object();

event.version = function(request){
  response_ok("Value: LINKJS_1.2.0.0\r\n");
}

event.OnBoot = function(request){
  response_ok("Value: " +
    "\\u\\s[-1]\\h\\s[0]" +
    "�d\\w5�d\\w5�d\\w5�d\\w9\\w9\\n" +
    "�[�I�Ɍ����΁A\\w5�Ȃ���Ƃ������́A" +
    "\\w9�\�����L����Ƃ������Ƃ��B\\n\\n[half]" +
    "\\w9\\w9�d\\w5�d\\w5�d\\w9\\w9���^�V�̉\���́A\\w5���N�ɂ������Ă���B" +
    "\\w9\\w9\\w9\\n\\n[half]" +
    "�d\\w5�d\\w5�d\\w5�d\\w9\\n" +
    "���͗���ׂ������ɔ����A" +
    "\\w5����ɂ��Ƃ��悤�B" +
    "\\n\\w9\\w9�d\\w5�d\\w5�N�ɁA\\w5������B" +
    "\\w9\\w9\\w9\\-" +
    "\r\n");
}
event.OnFirstBoot    = event.OnBoot;
event.OnGhostChanged = event.OnBoot;


//=============================================================
// ���C�����[�v
//=============================================================
while(true){
  var request = readRequest();
  if(request == null) WScript.Quit();
  switch(request.method){
  case null:
    response_bad_request("SHIORI 3.0�ł͂���܂���");
    continue;
  case "GET":
    var func = event[request.item["ID"]];
    if(func != null){
      func(request);
      continue;
    }
  }
  response_no_content("");
}


