# Contributing

<br/>

## Who can contribute

Everyone is welcome.

<br/>

## How to contribute

*If you don't know how to contribute to a project, read the [GitHub Guidelines](https://docs.github.com/en/get-started/quickstart/contributing-to-projects) first.*

<br/>

If you want to contribute to LogDoctor, please follow these steps:

  - Possibly open an **Issue** before to start, to declare what your contribution will be

  - **Fork** the `devel` branch of this project ([How?](#how-to-fork-the-devel-branch), [Why?](#why-the-devel-branch))

  - **Improve** it with your ideas

  - Submit a **Pull Request** to the origin branch, namely `elB4RTO/LogDoctor:devel`

<br/>

## Pull Requests

When submitting a PR, please follow the template.

<br/><br/>

## How to fork the DEVEL branch

You have two ways of doing it:

- while forking:
  - **uncheck** the "*Only copy the main branch*" option

*or*
     
- alternatively, you can do it afterwards:
  - step in your fork of the repository
  - select `branches` or click on `main` and select "*View all branches*"
  - tap `New branch`
  - insert the name you please and select `elB4RTO/LogDoctor` as repository and `devel` as branch

<br/>

## Why the DEVEL branch

It's related to the git system and mantainer's choice.<br/>
The `main` branch contains website-specific contents, which differ from site to site, while the `devel` branch is (supposed) to be the same and to act as an indipendent base for the code (and hopefully to keep a linear history).<br/>
As an example, if you push changes to the `main` branch on github, they won't appear as yours on gitlab (or whatever other site this repo is or will be hosted on) because the `devel` branch will **never** pull changes happened to `main`, meaning that changes will have to be copy-pasted to the `devel` branch and re-committed.

<br/>
