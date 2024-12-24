# 🌟 **Guide for Kosmic Contributors** 🌟

Hello developers! 💖 Thank you for wanting to help Kosmic become even more amazing!
This document is a summary of the most important points for anyone who wants to contribute, whether by reporting bugs, sending pull requests or helping in other ways. ✨

The Kosmic documentation has a section dedicated to [Contributions](https://docs.kosmicengine.org/en/latest/contributing/how_to_contribute.html), 
full of details and tips. It's really worth taking a look! 📖💡

## 🗂️ **Index**

- [Reporting bugs](#🐞-reporting-bugs)  
- [Proposing features or improvements](#🌟-proposing-features-or-improvements)  
- [Contributing with pull requests](#💻-contributing-with-pull-requests)  
- [Helping with translations](#🌍-helping-with-translations)  
- [Communicating with developers](#💬-communicating-with-developers)  

## 🐞 **Reporting bugs**

Found a bug? Visit [this link](https://github.com/kosmicengine/kosmic/issues/new?assignees=&labels=&template=bug_report.yml) 
to report it! But before sending, follow the template instructions and remember:

1. Include a **Minimum Reproduction Project (MRP)**, that is, a small project that shows the problem with the 
as few files as possible (and without the `.kosmic` folder, please 💾).
2. Test the bug on the latest version of Kosmic! You can download the latest versions [here](https://kosmicengine.org/download/archive/). 
This helps you know if the problem has already been resolved. 😉

If the bug is a regression (something that used to work but stopped working), mention the tested versions. 
This is super useful for investigation! 🔍

## 🌟 **Proposing features or improvements**

**Our main tracker is just for bugs!**  
Want to suggest an amazing feature? Go to the [Kosmic Proposals](https://github.com/kosmicengine/kosmic-proposals) repository
and follow the instructions in the README and the issue template. 💡✨

## 💻 **Contributing pull requests**

Before sending that sealing PR, remember a few things:

1. Make sure your idea is something that multiple developers might need.
2. Talk to other developers to discuss the best way to implement.
3. Even if your PR isn't accepted right away, it could help in the future, so it's worth a try!

For bug fixes, it's always good to discuss the solution before implementing it (if you're not completely sure). Check out the [PR Review Guide](https://docs.kosmicengine.org/en/latest/contributing/workflow/pr_review_guidelines.html) to better understand what we expect. 

### ✨ **Tips for killer PRs and commits**

- **Focus on one topic per PR.** It's easier to review and approve small, well-targeted changes.  
- **Avoid messy commits.** If possible, combine commits that fix simple bugs or improve the same piece of code before submitting the PR.  
- **Format your commit messages.** The title should be short (up to 72 characters) and direct, starting with words related to the context of your commit, such as "feat", "fix" or "docs". Examples:

  - feat(support): add iOS support in C#
  - fix(render): rendering error in `GLES3`
  - fix(core): fix `Object::has_method()` for script static methods
  - docs(editor): improvements editor documentation

**Ah!** If your PR includes new methods or properties, update the corresponding documentation. The [Class Reference](https://docs.kosmicengine.org/en/latest/contributing/documentation/updating_the_class_reference.html) must always be complete! ✍️📖

If your commit fixes an issue reported in `issues`, **include that information in the PR description** (not in the commit title or message), using one of the [GitHub closing keywords](https: //docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue), like "`Fixes #1234`". This causes the issue to be closed automatically when the PR is accepted.

**Attention!** Adding this information to the commit message may seem easier, but it fills the issue history with unnecessary updates, leaving the topic in a mess (and nobody likes that, right? 😅).  

### 💡 **Example of a well-formatted commit message**  

Here is a working example for you to get inspired. 
Note how the description was manually formatted to be readable, with line breaks every 80 characters:

```
Avoid turning French fries into charcoal by correcting the heat setting

When using the Kosmic potato frying module, the engine did not regulate the heat,
bringing the oil bath to supercritical liquid conditions. This had unexpected effects 
side effects in the physics engine (and some half-roasted... potatoes 💀🍟).

We fixed the regulation system by adding a link to the functionality
internal, ensuring that Kosmic now respects the boiling temperature of the
cooking oil under normal atmospheric conditions.
```

> [!IMPORTANT]
> If you use GitHub's online editor or its drag and drop feature, edit the commit title to follow the pattern, okay? Commits called "Update my_file.cpp" will not be accepted!

### 🧪 **Include tests in your PR!**

Unit tests are super important to prevent old problems from returning. They also give confidence that your contribution is ready to be integrated. If possible, include tests in the same commit that fixes the bug or adds the functionality. 💡

See [this section](https://docs.kosmicengine.org/en/latest/contributing/development/core_and_modules/unit_testing.html) to learn how to create tests for Kosmic.

## 🌍 **Helping with translations**

Kosmic is on [Hosted Weblate](https://hosted.weblate.org/projects/kosmicengine/), an open source platform for translations. 
We want to take Kosmic worldwide, so your help is greatly appreciated! 💕

Check out our [Localization Guide](https://docs.kosmicengine.org/en/latest/contributing/documentation/editor_and_docs_localization.html) 
to understand more about translating the editor and documentation. 🌐✨

## 💬 **Communicating with developers**

Need help or want to discuss an idea? We have several channels for this:  

- **[Discord Chat](https://discord.gg/QKApsehBCs):** This is where you find Kosmic's main developers. 
Take a look at channel directories to choose the best place for your question or idea. 💬  
- **[Bug tracker](https://github.com/kosmicengine/kosmic/issues):** For bug related topics.  
- **[Proposal Repository](https://github.com/kosmicengine/kosmic-proposals):** To suggest new features or improvements.

---

💖 Thank you very much for wanting to contribute to Kosmic! With your help, we can create an even more cosmic and powerful engine. 🎮✨  
— **Komii, Kosmic development team**