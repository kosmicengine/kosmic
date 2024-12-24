# 🌟 **Guia para Contribuidores da Kosmic** 🌟

Olá, desenvolvedores! 💖 Obrigada por querer ajudar a Kosmic a ficar ainda mais incrível!
Este documento é um resumão dos pontos mais importantes para quem quer contribuir, seja reportando bugs, enviando pull requests ou ajudando de outras formas. ✨

A documentação da Kosmic tem uma seção dedicada a [Contribuições](https://docs.kosmicengine.org/en/latest/contributing/how_to_contribute.html), 
cheia de detalhes e dicas. Vale muito a pena dar uma olhadinha! 📖💡

## 🗂️ **Índice**

- [Reportando bugs](#🐞-reportando-bugs)  
- [Propondo recursos ou melhorias](#🌟-propondo-recursos-ou-melhorias)  
- [Contribuindo com pull requests](#💻-contribuindo-com-pull-requests)  
- [Ajudando nas traduções](#🌍-ajudando-nas-traduções)  
- [Comunicando-se com os desenvolvedores](#💬-comunicando-se-com-os-desenvolvedores)  

## 🐞 **Reportando bugs**

Achou um bug? Acesse [este link](https://github.com/kosmicengine/kosmic/issues/new?assignees=&labels=&template=bug_report.yml) 
para reportá-lo! Mas antes de enviar, siga as instruções do template e lembre-se de:

1. Incluir um **Projeto de Reprodução Mínima (MRP)**, ou seja, um projetinho que mostre o problema com o 
mínimo de arquivos possível (e sem a pasta `.kosmic`, por favor 💾).
2. Testar o bug na última versão da Kosmic! Você pode baixar as versões mais recentes [aqui](https://kosmicengine.org/download/archive/). 
Isso ajuda a saber se o problema já foi resolvido. 😉

Se o bug for uma **regressão** (regression) (algo que funcionava antes, mas parou de funcionar), mencione as versões testadas. 
Isso é super útil para a investigação! 🔍

## 🌟 **Propondo recursos ou melhorias**

**Nosso rastreador principal é só para bugs!**  
Quer sugerir uma funcionalidade incrível? Vá até o repositório de [Propostas da Kosmic](https://github.com/kosmicengine/kosmic-proposals) 
e siga as instruções do README e do template de issues. 💡✨

## 💻 **Contribuindo com pull requests**

Antes de enviar aquele PR lacrador, lembre-se de algumas coisinhas:

1. Certifique-se de que sua ideia é algo que vários desenvolvedores possam precisar.
2. Converse com outros desenvolvedores para discutir a melhor forma de implementar.
3. Mesmo que seu PR não seja aceito de imediato, ele pode ajudar no futuro, então vale a pena tentar!

Para correções de bugs, é sempre bom discutir a solução antes de implementá-la (se você não tiver certeza absoluta). Confira o [Guia de Revisão de PRs](https://docs.kosmicengine.org/en/latest/contributing/workflow/pr_review_guidelines.html) para entender melhor o que esperamos. 

### ✨ **Dicas para PRs e commits arrasadores**  

- **Foque em um tema por PR.** É mais fácil revisar e aprovar mudanças pequenas e bem segmentadas.  
- **Evite commits bagunçados.** Se possível, combine commits que corrigem erros simples ou melhoram o mesmo trecho de código antes de enviar o PR.  
- **Formate suas mensagens de commit.** O título deve ser curto (até 72 caracteres) e direto, começando com palavras relacionadas ao contexto do seu commit, como "feat", "fix" ou "docs". Exemplos:

  - feat(support): add iOS support in C#
  - fix(render): rendering error in `GLES3`
  - fix(core): fix `Object::has_method()` for script static methods
  - docs(editor): improvements editor documentation

> [!WARNING]
> Lembre-se de sempre escrever em inglês!

**Ah!** Se seu PR inclui novos métodos ou propriedades, atualize a documentação correspondente. A [Referência de Classes](https://docs.kosmicengine.org/en/latest/contributing/documentation/updating_the_class_reference.html) precisa estar sempre completinha! ✍️📖

Se o seu commit corrige um problema relatado no `issues`, **inclua essa informação na descrição do PR** (não no título ou na mensagem do commit), usando uma das [palavras-chave de fechamento do GitHub](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue), como "`Fixes #1234`". Isso faz com que o problema seja fechado automaticamente quando o PR for aceito.  

**Atenção!** Adicionar essa informação na mensagem do commit pode parecer mais fácil, mas enche o histórico do problema com atualizações desnecessárias, deixando o tópico uma bagunça (e ninguém gosta disso, né? 😅).  

### 💡 **Exemplo de mensagem de commit bem formatada**  

Aqui está um exemplo funcional para você se inspirar. 
Note como a descrição foi formatada manualmente para ficar legível, com quebras de linha a cada 80 caracteres:

```
Avoid turning French fries into charcoal by correcting the heat setting

When using the Kosmic potato frying module, the engine did not regulate the heat,
bringing the oil bath to supercritical liquid conditions. This had effects unexpected 
side effects in the physics engine (and some half-roasted... potatoes 💀🍟).

We fixed the regulation system by adding a link to the functionality
internal, ensuring that Kosmic now respects the boiling temperature of the
cooking oil under normal atmospheric conditions.
```

> [!IMPORTANT]
> Caso utilize o editor online do GitHub ou seu recurso de arrastar e soltar, edite o título do commit para seguir o padrão, okay? Commits chamados "Update my_file.cpp" não serão aceitos!

### 🧪 **Inclua testes no seu PR!**

Testes unitários são super importantes para evitar que problemas antigos voltem. Eles também dão confiança de que sua contribuição está prontinha para ser integrada. Se for possível, inclua testes no mesmo commit que corrige o bug ou adiciona a funcionalidade. 💡

Veja [esta seção](https://docs.kosmicengine.org/en/latest/contributing/development/core_and_modules/unit_testing.html) para aprender como criar testes para a Kosmic.

## 🌍 **Ajudando nas traduções**

A Kosmic está no [Hosted Weblate](https://hosted.weblate.org/projects/kosmicengine/), uma plataforma open source para traduções. 
Queremos levar a Kosmic para o mundo inteiro, então sua ajuda é super bem-vinda! 💕

Confira nosso [Guia de Localização](https://docs.kosmicengine.org/en/latest/contributing/documentation/editor_and_docs_localization.html) 
para entender mais sobre como traduzir o editor e a documentação. 🌐✨

## 💬 **Comunicando-se com os desenvolvedores**

Precisa de ajuda ou quer discutir uma ideia? Temos vários canais para isso:  

- **[Chat do Discord - pt-BR](https://discord.gg/QKApsehBCs):** É onde você encontra os desenvolvedores principais da Kosmic. 
Dê uma olhada nos diretórios de canais para escolher o melhor lugar para sua dúvida ou ideia. 💬  
- **[Bug tracker](https://github.com/kosmicengine/kosmic/issues):** Para tópicos relacionados a bugs.  
- **[Repositório de Propostas](https://github.com/kosmicengine/kosmic-proposals):** Para sugerir novos recursos ou melhorias.  

---

💖 Muito obrigada por querer contribuir com a Kosmic! Com a sua ajuda, podemos criar uma engine ainda mais cósmica e poderosa. 🎮✨  
— **Komii, time de desenvolvimento da Kosmic**
