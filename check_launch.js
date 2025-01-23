
// This script is used to check if the executable is working properly.
// because the vscode extension need utf-8 encoding, but some windows console only support cp932 encoding, app output may be no readable characters in the console. 


function main() {

  const cmd = "build\\Release\\im-select-mspy.exe";
  const exec = require('child_process').exec;
  exec(cmd, (err, stdout) => {
    if (err) {
      console.error(err);
      return;
    }
    console.log(stdout);
  });
}

main();



