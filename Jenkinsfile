pipeline {
  agent any
  stages {
    stage('Init') {
      steps {
        echo 'Init done'
      }
    }
    stage('Builds') {
      parallel {
        stage('Windows Release|x86') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" "C:\\Program Files (x86)\\Jenkins\\workspace\\OpenHab2RulesGenerator\\OpenHab2RulesGenerator.sln" /p:configuration=Release /p:platform=x86 /t:rebuild'
          }
        }
        stage('Windows Debug|x86') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" "C:\\Program Files (x86)\\Jenkins\\workspace\\OpenHab2RulesGenerator\\OpenHab2RulesGenerator.sln" /p:configuration=Debug /p:platform=x86 /t:rebuild'
          }
        }
        stage('Windows Release|x64') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" "C:\\Program Files (x86)\\Jenkins\\workspace\\OpenHab2RulesGenerator\\OpenHab2RulesGenerator.sln" /p:configuration=Release /p:platform=x64 /t:rebuild'
          }
        }
        stage('Windows Debug|x64') {
          steps {
            bat '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe" "C:\\Program Files (x86)\\Jenkins\\workspace\\OpenHab2RulesGenerator\\OpenHab2RulesGenerator.sln" /p:configuration=Debug /p:platform=x64 /t:rebuild'
          }
        }
      }
    }
    stage('Test') {
      steps {
        echo 'Testing done'
      }
    }
    stage('Finish') {
      steps {
        echo 'Build done'
      }
    }
  }
}
