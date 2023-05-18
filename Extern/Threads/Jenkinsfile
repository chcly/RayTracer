pipeline {
    agent any
    triggers {
        GenericTrigger(causeString: 'Relay Push Service',  token: 'Thread.Build')
    }
    options {

        buildDiscarder logRotator(
            artifactDaysToKeepStr: '', 
            artifactNumToKeepStr: '', 
            daysToKeepStr: '1', 
            numToKeepStr: '10')
    }

    stages {

        stage('Update Submodules') {
            steps {
                sh script: 'git submodule init'
                sh script: 'git submodule update --init --merge'
            }
        }
        stage('Build Project') {

            steps {
                sh script: 'cmake -DThreads_BUILD_TEST=ON .'
                sh script: 'make'
            }
        }
        
        stage('Test Project') {
            steps {
                sh script: './Test/ThreadsTest'
            }
        }

        stage('Finalize') {
            steps {
                deleteDir()
            }
        }
    }
}